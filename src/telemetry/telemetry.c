/* -----------------------------------------------------------------------------
 * Component Name:
 * Author(s): 
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


/* prototypes declaration */
static void* thread_func(void*);

int init_telemetry(void *args){

    return create_thread("telemetry", thread_func, 20);
}

static void* thread_func(void* param){

    char buffer[1400];
    char output[1400];
    int bytes, n, packets_recieved = 0;

    while(1){

        read_e_link(buffer, 2);

        printf("New file inc\n");
        printf("read ret: %d\n", n);
        if(n == -1){
            printf("read error: %s\n", strerror(errno));
        }
        printf("Packet revieved - one -\n");
        printf("First bite: %x\n", buffer[0]);
        if(buffer[0]==0){
            printf("Its a message\n");
            for(int i = 1; buffer[i]!='\0' && i<1400;i++){
                output[i-1]=buffer[i];
            }
            printf("Message: %s\n", output);
        } else if(buffer[0]==1 && buffer[1]==0){
            printf("-File-\n");
            recieve_file(buffer);
            printf("Total packets recieved is: %d\n", packets_recieved);
            printf("-File recieved-\n");
        } else {
            printf("Unknown packet\n");
	}


    }


    return 1;   
}

int recieve_file(char buffer[1400]){

int fp, bytes_avail;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    char path[100] = "/home/zodox/Desktop/client_local/files/";
    unsigned short bytes, packets, current_package;

    read_e_link(buffer,6);

    current_package = *(unsigned short*)&buffer[0];
    packets = *(unsigned short*)&buffer[2];
    bytes = *(unsigned short*)&buffer[4];

    printf("Bytes: %d\n", bytes);
    printf("Packets: %d\n", packets);
    printf("Current package: %d\n", current_package);

    read_e_link(buffer,bytes);

    char* file_name = &buffer[0];
    strcat(path, file_name);
    printf("%s\n %s\n", file_name, path);
    //printf("Packets: %d\n",buffer[1]);
    fp = open(path, O_WRONLY | O_CREAT | O_APPEND, mode);
    printf("Open file: %s\n", path);
    //int packets = buffer[1];

    unsigned short ostkaka;

    printf("Total packets to read: %d\n", packets);
    for(int i = 0; i<packets;i++){
        //printf("-Packet_L: %d\n", i);

        read_e_link(buffer, 6);
        if(buffer[0]==0 && buffer[1]==0){
            close(fp);
            return 1;
        }
        if(buffer[0]==1 && buffer[1]==1){
            current_package = *(unsigned short*)&buffer[2];
            bytes = *(unsigned short*)&buffer[4];
            //printf("-Packet_S: %d\n", current_package);
            read_e_link(buffer, bytes);
            //printf("Bytes read: %d\n",ostkaka);
            //printf("Bytes intended: %d\n",bytes);
            write(fp, buffer, bytes);
        } else {
            printf("Error: %x\n", buffer[0]);
            printf("Error: %x\n", buffer[1]);
        }
    }
    close(fp);

    printf("-End of recieve_file()\n");

    return 1;
}