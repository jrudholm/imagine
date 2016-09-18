
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "imagine_api.h"
#include "common.h"

static struct opts {
	char *file;
} opts;
static int _verbose;

static void *get_lib_function(void *handle, char *name)
{
	char *errstr;
	void *function;

	function = dlsym(handle, name);
	errstr = dlerror();

	if (errstr || !function) {
		error("Could not get %s: %s\n", name, errstr);
		return NULL;
	}

	return function;
}

static void *open_lib(char *path)
{
	void *handle;
	int ret;
	get_version_t get_version;
	int my_version = __get_version();

	handle = dlopen(path, RTLD_LAZY);
	if (!handle) {
		error("Could not load %s: %s\n", path, dlerror());
		return NULL;
	}
	dlerror();

	/* Check API version */
	get_version = get_lib_function(handle, "__get_version");
	if (!get_version)
		goto err;

	ret = get_version();
	verbose("%s: API version %d\n", path, ret);

	if (ret != my_version) {
		error("Error when loading %s: API version mismatch (%d != %d)\n",
				path, my_version, ret);
		goto err;
	}

	return handle;

 err:
	dlclose(handle);
	return NULL;
}

static void close_lib(void *handle)
{
	if (handle)
		dlclose(handle);
}

void parse_opts(int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, "vl:h")) != -1) {
		switch (opt) {
		case 'v':
			_verbose = 1;
			break;
		case 'l':
			opts.file = strdup(optarg);
			break;
		case 'h':
		default:
			error("Usage: %s [OPTION]\n"
				"	-h Help\n"
				"	-v Verbose\n"
				"	-l <file.so> Load shared object file.so\n"
				"", argv[0]);
			exit(1);
		}
	}
}

#define SOURCE "lib/source.so"
#define ALGO "lib/libalgo1.so"

int main(int argc, char **argv)
{
	void *algo_handle = NULL;
	void *source_handle = NULL;
	run_t source_run;
	stop_t source_stop;
	setup_t algo_setup;
	int ret = 0;
	void *retval;
	pthread_t thread;

	parse_opts(argc, argv);

	/* Open source lib */
	source_handle = open_lib(SOURCE);
	if (!source_handle)
		exit(1);

	source_run = get_lib_function(source_handle, "run");
	if (!source_run) {
		ret = 1;
		goto out;
	}

	source_stop = get_lib_function(source_handle, "stop");
	if (!source_stop) {
		ret = 1;
		goto out;
	}

	/* Open algo lib */
	algo_handle = open_lib(ALGO);
	if (!algo_handle)
		exit(1);

	algo_setup = get_lib_function(algo_handle, "setup");
	if (!algo_setup) {
		ret = 1;
		goto out;
	}

	algo_setup(5);

	/* Call the run function from a thread */
	verbose("Calling run\n");
	pthread_create(&thread, NULL, source_run, NULL);

	verbose("Sleeping until Enter\n");

	getc(stdin);

	verbose("Stopping thread\n");
	source_stop();

	pthread_join(thread, &retval);

 out:
	close_lib(source_handle);
	close_lib(algo_handle);
	exit(ret);
}
