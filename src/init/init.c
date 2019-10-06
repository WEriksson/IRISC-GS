/* -----------------------------------------------------------------------------
 * Component Name: Init
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <gtk/gtk.h>

#include "e_link.h"
#include "telecommand.h"
#include "telemetry.h"
#include "global_utils.h"
#include "ui.h"

/* not including init */
#define MODULE_COUNT 3

static int init_func(int argc, char* const argv[]);

static char stderr_buf[4096];

static int ret;

/* This list controls the order of initialisation */
static const module_init_t init_sequence[MODULE_COUNT] = {
    //{"ui", &init_ui},
    {"e_link", &init_elink},
    {"telemetry", &init_telemetry},
    //{"telecommand", &init_telecommand},
    {"global_utils", &init_global_utils},
};

int main(int argc, char* argv[]){

    /* add buffer to stderr */
    if(setvbuf(stderr, stderr_buf, _IOLBF, 4096)){
        logging(ERROR, "INIT", "Failed to set buffer for stderr");
    }

    if(init_func(argc, argv)){
        return FAILURE;
    }

    init_ui(argc, argv);

    while(1){
        sleep(10);
    }
    return FAILURE;
}

static int init_func(int argc, char* const argv[]){

    int count = 0;
    for(int i=0; i<MODULE_COUNT; ++i){
        if(strcmp(init_sequence[i].name, "global_utils") == 0){
            ret = init_sequence[i].init(argv[0]);
        } else if (strcmp(init_sequence[i].name, "ui") == 0){
            ret = init_sequence[i].init((void*)&argc);
        }
        else{
            ret = init_sequence[i].init(NULL);
        }

        if( ret == SUCCESS ){
            logging(INFO, "INIT", "Module \"%s\" initialised successfully.",
                init_sequence[i].name);
        } else if( ret == FAILURE ){
            logging(ERROR, "INIT", "Module \"%s\" FAILED TO INITIALISE, return value: %d",
                init_sequence[i].name, ret);
            ++count;
        } else {
            logging(ERROR, "INIT", "Module \"%s\" FAILED TO INITIALISE, return value: %d, %s",
                init_sequence[i].name, ret, strerror(ret));
            ++count;
        }
    }

    logging(INFO, "INIT",
        "A total of %d modules initialised successfully and %d failed.",
        MODULE_COUNT-count, count);

    if (count != 0) {
        return FAILURE;
    }

    return SUCCESS;
}