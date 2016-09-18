
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

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

	handle = dlopen(path, RTLD_LAZY);
	if (!handle) {
		error("Could not load %s: %s\n", path, dlerror());
		return NULL;
	}
	dlerror();

	/* Check API version */
	get_version = get_lib_function(handle, "get_version");
	if (!get_version)
		goto err;

	ret = get_version();
	verbose("%s: API version %d\n", path, ret);

	if (ret != __IMAGINE_API_VERSION) {
		error("Error when loading %s: API version mismatch (%d != %d)\n",
				path, __IMAGINE_API_VERSION, ret);
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

int main(int argc, char **argv)
{
	void *handle;

	parse_opts(argc, argv);

	if (!opts.file) {
		error("No file to load\n");
		exit(1);
	}

	handle = open_lib(opts.file);
	if (handle) {
		close_lib(handle);
		exit(0);
	}

	exit(1);
}
