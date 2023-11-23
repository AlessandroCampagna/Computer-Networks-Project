#include "server.h"

int main(int argc, char *argv[]) {
    int ASport = -1;
    char ASportStr[6]; //TODO check if this is the right size
    int GN = GROUP_NUMBER;
    bool Verbose = false;

    int UDPsocket, TCPsocket, errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo UDPhints, TCPhints,*res;
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

    // Create socketS
    UDPsocket = socket(AF_INET, SOCK_DGRAM, 0); //UDP socket
    if(UDPsocket == -1) {
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    TCPsocket = socket(AF_INET, SOCK_STREAM, 0); //UDP socket
    if(TCPsocket == -1) {
        perror("Error creating TCP socket");
        exit(EXIT_FAILURE);
    }

    // Clear address structure
    memset(&UDPhints, 0, sizeof UDPhints);
    memset(&TCPhints, 0, sizeof TCPhints);

    // Define address structure for UDP
    UDPhints.ai_family = AF_INET; // IPv4
    UDPhints.ai_socktype = SOCK_DGRAM; // UDP socket
    UDPhints.ai_flags = AI_PASSIVE;

    // Define address structure for TCP
    TCPhints.ai_family = AF_INET; // IPv4
    TCPhints.ai_socktype = SOCK_STREAM; // TCP socket
    TCPhints.ai_flags = AI_PASSIVE;

    // Get UDP address info
    errcode = getaddrinfo(NULL, ASportStr, &UDPhints, &res);
    if(errcode == -1) {
        perror("Error getting address info");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket 
    n = bind(UDPsocket, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Get TCP address info
    errcode = getaddrinfo(NULL, ASportStr, &TCPhints, &res);
    if(errcode == -1) {
        perror("Error getting address info");
        exit(EXIT_FAILURE);
    }

    // Bind TCP socket 
    n = bind(TCPsocket, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Wait for requests
    while(true) {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message
	    addrlen = sizeof(addr);
	    n = recvfrom(UDPsocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &addrlen);
        if(n == -1) {
            perror("Error receiving message");
            exit(EXIT_FAILURE);
        }
    
        // Print received message    
        printf("received: %s\n", buffer);

        // Process request 
        ConnectionType connectionType = handle_request(buffer);
        if(errcode == -1) {
            perror("Error processing request");
            exit(EXIT_FAILURE);
        }

        // Echo back
        printf("sending: %s\n", buffer);
	    n = sendto(UDPsocket, buffer, n, 0, (struct sockaddr*) &addr, addrlen);
        if(n == -1) {
            perror("Error echoing message");
            exit(EXIT_FAILURE);
        }
    }

    freeaddrinfo(res);

    close(UDPsocket);
    close(TCPsocket);

    return 0;
}