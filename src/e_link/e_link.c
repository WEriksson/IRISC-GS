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

#define SERVER_IP "10.42.0.175"
#define SERVER_PORT 1337
#define SERVER_PORT_BACKUP 420

int sockfd;

void init_elink(void* args){

    int portno, n, ost;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0) 
        printf("ERROR opening socket");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
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
    return;
}

int send_e_link( char buffer[1400], int bytes){

    int n;

    n = write(sockfd, buffer, bytes);
    if (n < 0) {
         error("ERROR writing to socket");
    }

    return 1;
}

int read_e_link(char *buffer, int bytes){

    int n, bytes_avail;

    do{
        ioctl(sockfd, FIONREAD, &bytes_avail);
        sleep(1);
    } while(bytes_avail < bytes);

    n = read(sockfd, buffer, bytes);
    if(n == -1){
        printf("read error: %s\n", strerror(errno));
    }

    return n;
}