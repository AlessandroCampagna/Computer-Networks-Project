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

void* handle_UDP(char* ASportStr) {

    Connection UDPconnection;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    socklen_t addrlen;
    int n, errcode;

    char buffer[BUFFER_SIZE];

    // Initialize UDP connection
    UDPconnection.type = ConnectionType::UDP;

     // Create UDP socket
    UDPconnection.socket = socket(AF_INET, SOCK_DGRAM, 0); //UDP socket
    if(UDPconnection.socket == -1) {
        perror("(UDP) Error creating UDP socket");
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
        perror("(UDP) Error getting UDP address info");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket
    n = bind(UDPconnection.socket, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("(UDP) Error binding UDP socket");
        exit(EXIT_FAILURE);
    }

    // Place the UDP related code here
    while(true) {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message
        addrlen = sizeof(addr);
        n = recvfrom(UDPconnection.socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &addrlen);
        if(n == -1) {
            perror("(UDP) Error receiving message");
            exit(EXIT_FAILURE);
        }

        // Print received message    
        printf("(UDP) Received: %s", buffer);

        // Process request 
        ConnectionType connectionType = handle_request(buffer);
        if(errcode == -1) {
            perror("(UDP) Error processing request");
            exit(EXIT_FAILURE);
        }

        if (connectionType == ConnectionType::EXIT) {
            break;
        } else if (connectionType == ConnectionType::INVALID) {
            perror("(UDP) Error processing request");
            exit(EXIT_FAILURE);
        } else if (connectionType == ConnectionType::UDP) {
            printf("(UDP) Responding: %s\n", buffer);
            n = sendto(UDPconnection.socket, buffer, n, 0, (struct sockaddr*) &addr, addrlen);
            if(n == -1) {
                perror("(UDP) Error sending message");
                exit(EXIT_FAILURE);
            }
        }
    }

    close(UDPconnection.socket);
    freeaddrinfo(res);
    return NULL;
}

void* handle_TCP(char* ASportStr) {

    Connection TCPconnection;

    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    socklen_t addrlen;
    int n, errcode;

    char buffer[BUFFER_SIZE];

    // Initialize TCP connection
    TCPconnection.type = ConnectionType::TCP;

     // Create TCP socket
    TCPconnection.socket = socket(AF_INET, SOCK_STREAM, 0); //TCP socket
    if(TCPconnection.socket == -1) {
        perror("(TCP) Error creating TCP socket");
        exit(EXIT_FAILURE);
    }

    // Define address structure for TCP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;

    // Get TCP address info
    errcode = getaddrinfo(NULL, ASportStr, &hints, &res);
    if(errcode == -1) {
        perror("(TCP) Error getting TCP address info");
        exit(EXIT_FAILURE);
    }

    // Bind TCP socket
    n = bind(TCPconnection.socket, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        perror("(TCP) Error binding TCP socket");
        exit(EXIT_FAILURE);
    }

    // Place the TCP related code here
    while(true) {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Listen for connections
        if (listen(TCPconnection.socket, BACKLOG) == -1) {
            perror("(TCP)  Error listening on socket");
            exit(EXIT_FAILURE);
        }

        // Accept a connection
        addrlen = sizeof(addr);
        int new_socket = accept(TCPconnection.socket, (struct sockaddr*)&addr, &addrlen);
        if (new_socket == -1) {
            perror("(TCP)  Error accepting connection");
            exit(EXIT_FAILURE);
        }

        // Set timeout for receive operations
        struct timeval timeout;
        timeout.tv_sec = TIME_OUT;  // timeout after 5 seconds
        timeout.tv_usec = 0;  // not init'ing this can cause strange errors
        setsockopt(TCPconnection.socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

        // Receive message
        addrlen = sizeof(addr);
        n = recv(TCPconnection.socket, buffer, BUFFER_SIZE, 0);
        if(n == -1) {
            perror("(TCP)  Error receiving message");
            exit(EXIT_FAILURE);
        }

        // Print received message    
        printf("(TCP) Received: %s", buffer);

        // Process request 
        ConnectionType connectionType = handle_request(buffer);
        if(errcode == -1) {
            perror("(TCP)  Error processing request");
            exit(EXIT_FAILURE);
        }

        if (connectionType == ConnectionType::EXIT) {
            break;
        } else if (connectionType == ConnectionType::INVALID) {
            perror("(TCP)  Error processing request");
            exit(EXIT_FAILURE);
        } else if (connectionType == ConnectionType::TCP) {
            printf("(TCP) Sending: %s\n", buffer);
            n = send(TCPconnection.socket, buffer, n, 0);
            if(n == -1) {
                perror("(TCP) Error sending message");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    close(TCPconnection.socket);
    freeaddrinfo(res);
    return NULL;
}

int main(int argc, char *argv[]) {
    int ASport = -1;
    char ASportStr[5];
    int GN = GROUP_NUMBER;

    bool Verbose = false;

    int errcode;
    
    // Parse arguments
    errcode = parseArgs(argc, argv, &ASport, &GN, &Verbose);
    if (errcode == -1) {
        perror("(SERVER) Error parsing arguments");
        exit(EXIT_FAILURE);
    }

    // Default settings
    if (ASport == -1) ASport = PORT + GN; // default port
    sprintf(ASportStr, "%d", ASport); // convert port to string

    if (Verbose) printf("(SERVER) ASport = %s\n", ASportStr);

    // Create child processes for UDP and TCP
    pid_t udp, tcp;

    udp = fork();
    if (udp == -1) {
        perror("(SERVER) Error creating UDP child process");
        exit(EXIT_FAILURE);
    } else if (udp == 0) {
        // Child process for UDP
        handle_UDP(ASportStr);
        exit(EXIT_SUCCESS);
    }

    tcp = fork();
    if (tcp == -1) {
        perror("(SERVER) Error creating TCP child process");
        exit(EXIT_FAILURE);
    } else if (tcp == 0) {
        // Child process for TCP
        handle_TCP(ASportStr);
        exit(EXIT_SUCCESS);
    }

    // Wait for child process to finish
    int status;
    waitpid(udp, &status, 0);
    waitpid(tcp, &status, 0);

    return EXIT_SUCCESS;
}