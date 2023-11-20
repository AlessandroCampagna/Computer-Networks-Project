#include "user.h"

int main(int argc, char *argv[]) {
    char *ASIP = NULL;
    int ASport = -1;
    char ASportStr[6]; //TODO check if this is the right size

    int fd_udp, fd_tcp, errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints_udp, hints_tcp, *res_udp, *res_tcp;
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
    if (ASport == -1) ASport = PORT; // default port
    sprintf(ASportStr, "%d", ASport); // convert port to string

    fd_udp=socket(AF_INET,SOCK_DGRAM,0); //UDP socket

    memset(&hints_udp,0,sizeof hints_udp);
    hints_udp.ai_family=AF_INET; //IPv4
    hints_udp.ai_socktype=SOCK_DGRAM; //UDP socket

    errcode = getaddrinfo(ASIP,ASportStr,&hints_udp,&res_udp); if(errcode!=0) /*error*/ exit("EXIT_FAILURE");

    while(true){

        fgets(buffer, sizeof(buffer), stdin);
    
        ConnectionType connectionType = handle_command(buffer);

        if (connectionType == UDP) {

            n=sendto(fd_udp,buffer,strlen(buffer),0,res_udp->ai_addr,res_udp->ai_addrlen);
            if(n==-1) /*error*/ exit("EXIT_FAILURE");
    
            memset(&buffer,0,sizeof buffer);
            addrlen=sizeof(addr);

            n=recvfrom(fd_udp,buffer,BUFFER_SIZE,0,(struct sockaddr*)&addr,&addrlen);
            if(n==-1) /*error*/ exit("EXIT_FAILURE");

            handle_response(buffer);

        } else if (connectionType == TCP) {

            fd_tcp=socket(AF_INET,SOCK_STREAM,0); //TCP socket

            memset(&hints_tcp,0,sizeof hints_tcp);
            hints_tcp.ai_family=AF_INET; //IPv4
            hints_tcp.ai_socktype=SOCK_STREAM; //TCP socket

            errcode = getaddrinfo(ASIP,ASportStr,&hints_tcp,&res_tcp); if(errcode!=0) /*error*/ exit("EXIT_FAILURE");

            n=connect(fd_tcp,res_tcp->ai_addr,res_tcp->ai_addrlen);
            if(n==-1) /*error*/ exit("EXIT_FAILURE");

            n=write(fd_tcp, buffer, strlen(buffer));
            if(n==-1) /*error*/ exit("EXIT_FAILURE");

            memset(&buffer,0,sizeof(buffer));

            n=read(fd_tcp, buffer, BUFFER_SIZE);
            if(n==-1) /*error*/ exit("EXIT_FAILURE");

        } else if (connectionType == EXIT){
            if (!logged) exit(1);
            else printf("You must logout before exiting\n");

        } else {
            printf("Invalid Command\n");
        }
    }

    free(ASIP);
    return 0;
}
