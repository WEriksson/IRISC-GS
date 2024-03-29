/* -----------------------------------------------------------------------------
 * Component Name: Telecommand
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */

#define CMD_ENCODER 0
#define CMD_ROT_CYCLE 1
#define CMD_REBOOT 10
#define CMD_DATARATE 20
#define CMD_MODE 30
#define CMD_PING 40
#define CMD_STAR 50
#define CMD_NIR_EXP 60
#define CMD_NIR_GAI 65
#define CMD_ST_EXP 70
#define CMD_ST_GAI 75
#define CMD_STP_AZ 80
#define CMD_STP_ALT 85
#define CMD_CENTER 95
#define CMD_AZ_ERR 100
#define CMD_ALT_ERR 105
#define CMD_STOP_MOTORS 110
#define CMD_START_MOTORS 115


int init_telecommand(void* args);
int check_command(const char command[20], const char value[20]);