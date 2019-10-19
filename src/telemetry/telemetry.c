/* -----------------------------------------------------------------------------
 * Component Name: Telemetry
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> // strlen()
#include <unistd.h> //sleep
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

#include "e_link.h"
#include "global_utils.h"
#include "ui.h"


/* prototypes declaration */
static void* thread_func(void*);
int recieve_file(char buffer[1400]);
int recieve_message(char buffer[1400]);
int logg_message(char buffer[1400], unsigned short bytes);

int init_telemetry(void *args){

    return create_thread("telemetry", thread_func, 20, args);
    printf("BIG OOOF\n");
}

static void* thread_func(void* param){

    logging(INFO, "telemetry", "Start of TM thread");

    sleep(1);

    char buffer[1400];
    char output[1400];
    int packets_recieved = 0;

    while(1){
        memset(buffer, 0, sizeof(buffer));
        memset(output, 0, sizeof(output));

        read_e_link(buffer, 2, 1);

        logging(DEBUG, "telemetry", "New telemetry inc\n");
        //printf("First bite: %x\n", buffer[0]);
        if(buffer[0]==0){
            printf("Its a message\n");
            
            recieve_message(buffer);

        } else if(buffer[0]==1 && buffer[1]==0){
            printf("-It's a File-\n");
            recieve_file(buffer);
            printf("Total packets recieved is: %d\n", packets_recieved);
            printf("-File recieved-\n");
        } else {
            printf("Unknown packet\n");
	    }
    }

    return NULL;   
}

int recieve_file(char buffer[1400]){

    char temp[512];
    char time[9];
    char filename[30];

    get_time(time);

    strcpy(temp, time);
    strcat(temp, ": Incoming file: ");


    printf("recieve_file()\n");

    //char temp[256] = "INFO: Incoming file: ";

    int fp, n;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    //char path[100] = "/home/zodox/Desktop/IRISC-GS-FILES/IMAGES/";
    char path[100] = "../IRISC-GS-FILES/IMAGES/";
    
    unsigned short bytes, packets, current_package;
    read_e_link(buffer,6,0);

    current_package = *(unsigned short*)&buffer[0];
    packets = *(unsigned short*)&buffer[2];
    bytes = *(unsigned short*)&buffer[4];

    //printf("Bytes: %d\n", bytes);
    //printf("Packets: %d\n", packets);
    //printf("Current package: %d\n", current_package);

    read_e_link(buffer,bytes, 0);

    char* file_name = &buffer[0];

    strcpy(filename, file_name);

    strcat(temp, filename);
    strcat(temp, "\n");


    pthread_mutex_lock(&downlink_mutex);
    strcat(temp, tm_buffer);
    strcpy(tm_buffer, temp);
    *tm_update_flag = 1;

    pthread_mutex_unlock(&downlink_mutex);
    //strcpy(temp, file_name);

    //print_telemetry("File_Name");

    strcat(path, file_name);
    printf("%s\n %s\n", file_name, path);
    printf("Packets: %d\n",buffer[1]);
    fp = open(path, O_WRONLY | O_CREAT | O_APPEND, mode);
    //printf("Open file: %s\n", path);
    //int packets = buffer[1];

    //printf("Total packets to read: %d\n", packets);

    printf("--end--\n");


    for(int i = 0; i<packets;i++){
        //printf("-Packet_L: %d\n", i);

        read_e_link(buffer, 6, 1);
        if(buffer[0]==0 && buffer[1]==0){
            close(fp);
            return 1;
        }
        if(buffer[0]==1 && buffer[1]==1){
            current_package = *(unsigned short*)&buffer[2];
            bytes = *(unsigned short*)&buffer[4];
            //printf("-Packet_S: %d\n", current_package);
            read_e_link(buffer, bytes, 1);
            //printf("Bytes read: %d\n",ostkaka);
            //printf("Bytes intended: %d\n",bytes);
            n = write(fp, buffer, bytes);
            if(n==-1){
                printf("Error on write to file\n");
            }
        } else {
            printf("Error: %x\n", buffer[0]);
            printf("Error: %x\n", buffer[1]);
        }
    }
    close(fp);

    //strcpy(temp, "INFO: Recieved file: ");

    //strcat(temp, file_name);

    get_time(time);

    strcpy(temp, time);
    strcat(temp, ": Recieved file: ");
    strcat(temp, filename);
    strcat(temp, "\n");


    pthread_mutex_lock(&downlink_mutex);
    strcat(temp, tm_buffer);
    strcpy(tm_buffer, temp);
    *tm_update_flag = 1;

    pthread_mutex_unlock(&downlink_mutex);

    printf("-End of recieve_file\n");

    return 1;
}

int recieve_message(char buffer[1400]){

    unsigned short bytes;

    char temp[512];
    char time[9];

    get_time(time);

    strcpy(temp, time);
    strcat(temp, ": ");

    //char temp[256] = "Telemetry: ";

    read_e_link(buffer, 2, 0);
    bytes = *(unsigned short*)&buffer[0];
    
    read_e_link(buffer, bytes, 0);
    logging(INFO, "telemetry", "Message: %s", buffer);

    //strcpy(temp, buffer);

    strcat(temp, buffer);
    strcat(temp, "\n");
    pthread_mutex_lock(&downlink_mutex);
    strcat(temp, tm_buffer);
    printf("temp: %s\n", temp);
    //strcat(tm_buffer, temp);
    strcpy(tm_buffer, temp);
    *tm_update_flag = 1;
    pthread_mutex_unlock(&downlink_mutex);

    //print_telemetry("Message");


    logg_message(buffer, bytes);


    return SUCCESS;
}

int logg_message(char buffer[1400], unsigned short bytes){

    //TODO: logg to file

    return SUCCESS;
}