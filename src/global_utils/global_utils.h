/* -----------------------------------------------------------------------------
 * Component Name: Global Utils
 * Author(s): Adam Śmiałek, Harald Magnusson, William Eriksson
 * Purpose: Define utilities and constants available to all components.
 *
 * -----------------------------------------------------------------------------
 */

#pragma once

/* int function return values */
#define SUCCESS 0
#define FAILURE -1

/* definitions for socket creation */
#define SERVER_IP "10.42.0.175"
#define SERVER_PORT 1337
#define SERVER_PORT_BACKUP 420

#define DEBUG   0
#define INFO    1
#define WARN    2
#define ERROR   3
#define CRIT    4

/* struct used for initialisation of modules */
typedef int (*init_function)(void* args);
typedef struct {
    const char *name;
    init_function init;
} module_init_t;

/* initialise the global utils component */
int init_global_utils(void* args);

/* top_dir is the absolute path for the top directory
 * of the project evaluated at runtime.
 * get_top_dir returns a pointer to a string containing that directory.
 */
const char* const get_top_dir(void);

int init_submodules(const module_init_t init_sequence[], int module_count);

int logging(int level, char module_name[12],
            const char * format, ... );

/* a call to pthread_create with additional thread attributes,
 * specifically priority
 */
int create_thread(char* comp_name, void *(*thread_func)(void *), int prio, void *args);

/*
 *  Sets buffer to time in format "HH:MM:SS"
 */
void get_time(char *buffer);

/*
 *  logg tm in file
 */
void loggfile_tm(char *buffer);

/*
 *  logg tm in file
 */
void loggfile_tc(char *buffer);
