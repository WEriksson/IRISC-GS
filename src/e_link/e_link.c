/* -----------------------------------------------------------------------------
 * Component Name: E_link
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "global_utils.h"

int sockfd;

int init_elink(void* args){

    struct sockaddr_in serv_addr;
    struct hostent *server;
    while(1){
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (0) {
            printf("ERROR opening socket");
            continue;
        }
        server = gethostbyname(SERVER_IP);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            sleep(5);
            continue;
        }
        break;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(SERVER_PORT);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        printf("ERROR connecting, trying backup port\n");
        serv_addr.sin_port = htons(SERVER_PORT_BACKUP);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
            printf("ERROR connecting on backup port\n");
        }
    }
    return SUCCESS;
}

int send_e_link( char buffer[1400], int bytes){

    int n;

    n = write(sockfd, buffer, bytes);
    if (n < 0) {
         printf("ERROR writing to socket");
    }

    return 1;
}

int read_e_link(char *buffer, int bytes, int check_buffer){

    int n, bytes_avail;

    if(check_buffer){
        ioctl(sockfd, FIONREAD, &bytes_avail);
        while(bytes_avail < bytes){
            usleep(1);
            ioctl(sockfd, FIONREAD, &bytes_avail);
            //printf("Ostkaka\n");
        }
    }
/*
    printf("Data read: ");
    for (size_t i = 0; i < bytes; i++)
    {
        printf("%x", buffer[i]);
    }
    printf("\n");
*/
    n = read(sockfd, buffer, bytes);
    if(n == -1 || n == 0){
        printf("read error: %s\n", strerror(errno));
    }

    return n;
}