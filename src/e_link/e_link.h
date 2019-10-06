/* -----------------------------------------------------------------------------
 * Component Name: E_link
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */

int init_elink(void* args);

int send_e_link(char buffer[1400], int bytes);

int read_e_link(char *buffer, int bytes, int check_buffer);