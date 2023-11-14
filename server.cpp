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

#define TRUE 1
#define BUFFER_SIZE 128
#define PORT 58000

int main(int argc, char *argv[]) {
    int ASport = -1;
    char ASportStr[6]; //TODO check if this is the right size
    int GN = 17;
    bool Verbose = false;

    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    int opt;
    while ((opt = getopt(argc, argv, "p:v")) != -1) {
        switch (opt) {
        case 'p':
            ASport = atoi(optarg);
            break;
        case 'v':
            Verbose = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-p ASport] [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Default settings
    if (ASport == -1) ASport = PORT + GN; // default port
    sprintf(ASportStr, "%d", ASport); // convert port to string

    if (Verbose) printf("ASport = %s\n", ASportStr);

    // Establish UDP connection with cliennts

    // Create socket
    fd = socket(AF_INET, SOCK_DGRAM, 0); //UDP socket
    if(fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Clear address structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    hints.ai_flags = AI_PASSIVE;

    // Get address info
    errcode = getaddrinfo(NULL, ASportStr, &hints, &res);
    if(errcode == -1) {
        perror("Error getting address info");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    n = bind(fd, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Wait for requests
    while(TRUE) {
        // Receive message
	    addrlen = sizeof(addr);
	    n = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &addrlen);
        if(n == -1) {
            perror("Error receiving message");
            exit(EXIT_FAILURE);
        }
    
        // Print received message    
        buffer[n] = '\0'; // Ensure the buffer is null-terminated
        printf("received: %s\n", buffer);

        // Echo back
	    n = sendto(fd, buffer, n, 0, (struct sockaddr*) &addr, addrlen);
        if(n == -1) {
            perror("Error echoing message");
            exit(EXIT_FAILURE);
        }
    }

    freeaddrinfo(res);
    close(fd);

    return 0;
}