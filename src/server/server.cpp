#include "server.h"

int parseArgs(int argc, char *argv[], int *ASport, int *GN, bool *Verbose) {
    int opt;
    while ((opt = getopt(argc, argv, "p:v")) != -1) {
        switch (opt) {
        case 'p':
            *ASport = atoi(optarg);
            break;
        case 'v':
            *Verbose = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-p ASport] [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int initConnections(Connection *UDPconnection, Connection *TCPconnection, char *ASportStr)  {
    
    struct addrinfo hints, *res;
    int n, errcode;

    // Initialize UDP connection
    UDPconnection->type = ConnectionType::UDP;
    TCPconnection->type = ConnectionType::TCP;

     // Create UDP socket
    UDPconnection->socket = socket(AF_INET, SOCK_DGRAM, 0); //UDP socket
    if(UDPconnection->socket == -1) {
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    // Define address structure for UDP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    hints.ai_flags = AI_PASSIVE;

    // Get UDP address info
    errcode = getaddrinfo(NULL, ASportStr, &hints, &res);
    if(errcode == -1) {
        perror("Error getting UDP address info");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket
    n = bind(UDPconnection->socket, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("Error binding UDP socket");
        exit(EXIT_FAILURE);
    }

     // Create TCP socket
    TCPconnection->socket = socket(AF_INET, SOCK_STREAM, 0); //UDP socket
    if(TCPconnection->socket == -1) {
        perror("Error creating TCP socket");
        exit(EXIT_FAILURE);
    }

    // Define address structure for UDP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;

    // Get TCP address info
    errcode = getaddrinfo(NULL, ASportStr, &hints, &res);
    if(errcode == -1) {
        perror("Error getting TCP address info");
        exit(EXIT_FAILURE);
    }

    // Bind TCP socket
    n = bind(TCPconnection->socket, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("Error binding UDP socket");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    return 0;
}

int main(int argc, char *argv[]) {
    int ASport = -1;
    char ASportStr[5];
    int GN = GROUP_NUMBER;
    bool Verbose = false;

    int errcode;
    ssize_t n;

    Connection UDPconnection, TCPconnection;
    socklen_t addrlen;
    struct sockaddr_in addr;
    
    char buffer[BUFFER_SIZE];

    // Parse arguments
    errcode = parseArgs(argc, argv, &ASport, &GN, &Verbose);
    if (errcode == -1) {
        perror("Error parsing arguments");
        exit(EXIT_FAILURE);
    }

    // Default settings
    if (ASport == -1) ASport = PORT + GN; // default port
    sprintf(ASportStr, "%d", ASport); // convert port to string

    if (Verbose) printf("ASport = %s\n", ASportStr);

    // Initialize connections
    initConnections(&UDPconnection, &TCPconnection, ASportStr);

    // Wait for requests
    while(true) {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message
	    addrlen = sizeof(addr);
	    n = recvfrom(UDPconnection.socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &addrlen);
        if(n == -1) {
            perror("Error receiving message");
            exit(EXIT_FAILURE);
        }
    
        // Print received message    
        printf("(UDP) Received: %s", buffer);

        // Process request 
        ConnectionType connectionType = handle_request(buffer);
        if(errcode == -1) {
            perror("Error processing request");
            exit(EXIT_FAILURE);
        }

        if (connectionType == ConnectionType::EXIT) {
            break;
        } else if (connectionType == ConnectionType::INVALID) {
            perror("Error processing request");
            exit(EXIT_FAILURE);
        } else if (connectionType == ConnectionType::UDP) {
            printf("(UDP) Responding: %s\n", buffer);
            n = sendto(UDPconnection.socket, buffer, n, 0, (struct sockaddr*) &addr, addrlen);
            if(n == -1) {
                perror("Error sending message");
                exit(EXIT_FAILURE);
            }
        } else if (connectionType == ConnectionType::TCP) {
            printf("(TCP) Sending: %s\n", buffer);
            n = send(TCPconnection.socket, buffer, n, 0);
            if(n == -1) {
                perror("Error sending message");
                exit(EXIT_FAILURE);
            }
        }
    }

    close(UDPconnection.socket);
    close(TCPconnection.socket);

    return 0;
}