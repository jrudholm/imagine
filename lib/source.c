#include <stdio.h>
#include <unistd.h>
#include "../imagine_api.h"

static int stop_running = 0;

int setup(int val)
{
	printf("setup for source\n");
	return 0;
}

void *run(void *data)
{
	int i = 0;

	while (!stop_running) {
		printf("%d ", i);
		fflush(stdout);
		i++;
		sleep(1);
	}

	return NULL;
}

int stop(void)
{
	stop_running = 1;

	return 0;
}
