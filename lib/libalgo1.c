#include <stdio.h>
#include "../imagine_api.h"

static int setup_val;

int setup(int val)
{
	
	printf("setup for algo 1, val = %d\n", val);
	setup_val = val;

	return 0;

}

