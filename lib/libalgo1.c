#include <stdio.h>
#include "../imagine_api.h"

static int setup_val;

int setup(int val)
{
	
	printf("setup for algo 1\n");
	setup_val = val;

	return 0;

}

int get_version(void)
{
	return __IMAGINE_API_VERSION;

}
