#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <cstdio>

#define PORT 58000
#define BUFFER_SIZE 128

int main(int argc, char *argv[]) {
    char *ASIP = NULL;
    int ASport = -1;
    char ASportStr[6]; //TODO check if this is the right size
    int GN = 17;
    int uid;
    int aid;
    bool logged = false;

    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    int opt;
    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
        case 'n':
            ASIP = strdup(optarg);
            break;
        case 'p':
            ASport = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-n ASIP] [-p ASport]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Default settings
    if (ASIP == NULL) ASIP = strdup("localhost"); // default IP
    if (ASport == -1) ASport = PORT + GN; // default port
    sprintf(ASportStr, "%d", ASport); // convert port to string

    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) /*error*/exit(1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_DGRAM; //UDP socket

    errcode=getaddrinfo(ASIP,ASportStr,&hints,&res);
    if(errcode!=0) /*error*/exit(1);

    fgets(buffer, sizeof(buffer), stdin);

    printf("Sending message to server...\n");
    n=sendto(fd,buffer,strlen(buffer),0,res->ai_addr,res->ai_addrlen);
    if(n==-1) /*error*/ exit(1);

    printf("Awaiting response from server...\n");
    memset(&buffer,0,sizeof(buffer));
    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,BUFFER_SIZE,0,(struct sockaddr*)&addr,&addrlen);
    if(n==-1) /*error*/ exit(1);

    write(1,"echo: ",6); write(1,buffer,n);


    free(ASIP);
    return 0;
}
