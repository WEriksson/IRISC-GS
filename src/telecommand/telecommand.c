/* -----------------------------------------------------------------------------
 * Component Name: Telecommand
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "global_utils.h"
#include "telecommand.h"
#include "e_link.h"

int init_telecommand(void* args){

    return SUCCESS;
}

int check_command(char command[20], char value[20]){

    printf("Command: %s\n", command);
    printf("Value: %s\n", value);
    char cmd[1];
    //char cmd[3];
    char buffer[20];
    char temp[3];
    int ret;

    //ret = strtoi(value, NULL, 10);

    if(!strcmp(command,"Datarate")){            //Datarate
        cmd[0] = CMD_DATARATE;

        unsigned short datarate = (unsigned short) strtoul(value, NULL, 10);

        char* bytes = (char*)&datarate;
        buffer[0] = bytes[0];
        buffer[1] = bytes[1];

        printf("cmd: %s\n", cmd);
        printf("value: %s\n", value);

        if(datarate>0){
            send_e_link(cmd, 1);
            send_e_link(buffer, 2);
        }

        printf("Datarate sent\n");

        printf("Set Datarate to: %d\n", ret);
    } else if (!strcmp(command,"Reboot")){    //Reboot
        //*cmd = CMD_REBOOT;
        printf("Case Shutdown\n");
    } else if (!strcmp(command,"Ping")){        //Ping
        cmd[0] = CMD_PING;
        printf("Ping sent\n");
        send_e_link(cmd, 1);
        printf("Case Ping\n");
    } else if (!strcmp(command,"Mode")){        //Mode
        //*cmd = CMD_MODE;
        send_e_link(cmd, 2);
        printf("Case Mode\n");
    } else if (!strcmp(command,"ST Image")){        //STAR
        //*cmd = CMD_STAR;
        printf("Case STAR\n");
    } else if (!strcmp(command,"NIR Exposure")){        //NIR_EXP
        //*cmd = CMD_NIR_EXP;
        printf("Case NIR_EXP\n");
    } else if (!strcmp(command,"NIR Gain")){        //NIR_GAI
        //*cmd = CMD_NIR_GAI;
        printf("Case NIR_GAI\n");
    } else if (!strcmp(command,"ST Exposure")){        //ST_EXP
        //*cmd = CMD_ST_EXP;
        printf("Case ST_EXP\n");
    } else if (!strcmp(command,"ST Gain")){        //ST_GAI
        //*cmd = CMD_ST_GAI;
        printf("Case ST_GAI\n");
    } else {
        printf("Else\n");
        
        return FAILURE;
    }


    return SUCCESS;
}