/* -----------------------------------------------------------------------------
 * Component Name: ui
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */
char *tm_buffer;
int *tm_update_flag;

extern pthread_mutex_t downlink_mutex;

int init_ui(int argc, char *argv[]);

int print_telemetry(char buffer[256]);