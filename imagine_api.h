#ifndef _IMAGINE_H
#define _IMAGINE_H

/* Increment this when the API has changed */
#define __IMAGINE_API_VERSION	3

int __get_version(void)
{
	return __IMAGINE_API_VERSION;
}

/* Common dynamic library interfaces */
typedef int (*get_version_t)(void);


typedef void * (*run_t)(void *);
typedef int (*setup_t)(int);
typedef int (*stop_t)(void);


/* Sensor backend functions */
typedef int (*set_exp_lines_t)(int);

struct sensor_ops {
	set_exp_lines_t set_exp_lines;
};

/* Driver bacekdn functions */
typedef int (*get_stat_t)(int);


struct want {
	int what;
	char *when;
};


#endif
