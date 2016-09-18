#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

#define verbose(...) do { \
	if (_verbose) \
		fprintf(stdout, ##__VA_ARGS__); \
	} while (0)

#define error(...) fprintf(stderr, ##__VA_ARGS__)
#define info(...) printf(##__VA_ARGS__)

#endif
