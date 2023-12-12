#include "server.hpp"

//TODO: Handle SIG_PIPE and zombie processes 
//      Handle CTRL+C (SIGINT) and CTRL+Z (SIGTSTP) exit with grace

int parseArgs(int argc, char *argv[], int *ASport, int *GN, bool *Verbose)
{
    int opt;
    while ((opt = getopt(argc, argv, "p:v")) != -1)
    {
        switch (opt)
        {
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

void *handleUDP(char *ASportStr)
{
    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    int UDPsocket;
    socklen_t addrlen;

    int n, errcode;
    char buffer[UDP_BUFFER_SIZE];

    // Create UDP socket
    UDPsocket = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (UDPsocket == -1)
    {
        perror("(UDP) Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    // Define address structure for UDP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    hints.ai_flags = AI_PASSIVE;

    // Get UDP address info
    errcode = getaddrinfo(NULL, ASportStr, &hints, &res);
    if (errcode == -1)
    {
        perror("(UDP) Error getting UDP address info");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket
    n = bind(UDPsocket, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        perror("(UDP) Error binding UDP socket");
        exit(EXIT_FAILURE);
    }

    // Place socket in listening mode
    while (true)
    {
        // Clear buffer
        memset(buffer, 0, UDP_BUFFER_SIZE);

        // Receive message
        addrlen = sizeof(addr);
        n = recvfrom(UDPsocket, buffer, UDP_BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addrlen);
        if (n == -1)
        {
            perror("(UDP) Error receiving message");
            exit(EXIT_FAILURE);
        }

        // Print received message
        printf("(UDP) Received: %s", buffer);

        // Process request
        Command status = handleRequest(buffer);

        if (status == Command::COMMAND_NOT_FOUND)
        {
            perror("(UDP) Command not found");
            exit(EXIT_FAILURE);
        }
        
        printf("(UDP) Responding: %s\n", buffer);
        n = sendto(UDPsocket, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, addrlen);
        if (n == -1)
        {
            perror("(UDP) Error sending message");
            exit(EXIT_FAILURE);
        }

    }

    close(UDPsocket);
    freeaddrinfo(res);
    return NULL;
}

void *handleTCP(char *ASportStr)
{
    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    int parentSocket;
    socklen_t addrlen;
    
    int n, errcode;
    char buffer[TCP_BUFFER_SIZE];

    // Create TCP socket for incoming connections
    parentSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (parentSocket == -1)
    {
        perror("(TCP) Error creating TCP socket");
        exit(EXIT_FAILURE);
    }

    // Define address structure for TCP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;

    // Get TCP address info
    errcode = getaddrinfo(NULL, ASportStr, &hints, &res);
    if (errcode == -1)
    {
        perror("(TCP) Error getting TCP address info");
        exit(EXIT_FAILURE);
    }

    // Bind TCP socket
    n = bind(parentSocket, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    {
        perror("(TCP) Error binding TCP socket");
        exit(EXIT_FAILURE);
    }

    // Enter listening mode
    while (true)
    {
        // Clear buffer
        memset(buffer, 0, TCP_BUFFER_SIZE);

        // Listen for connections
        if (listen(parentSocket, BACKLOG) == -1)
        {
            perror("(TCP)  Error listening on socket");
            exit(EXIT_FAILURE);
        }

        // Accept a connection and create a socket for it
        addrlen = sizeof(addr);
        int childSoket = accept(parentSocket, (struct sockaddr *)&addr, &addrlen);
        if (childSoket == -1)
        {
            perror("(TCP)  Error accepting connection");
            exit(EXIT_FAILURE);
        }

        // Fork a new process to handle the request
        printf("(TCP)  Forking a new process for incoming request\n");
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("(TCP) Error creating a new process");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // Child process to handle request
        {
            close(parentSocket); // Close the parent socket for child

            // Set timeout for receive operations
            struct timeval timeout;
            timeout.tv_sec = TIME_OUT; // timeout after 5 seconds
            timeout.tv_usec = 0;       // not init'ing this can cause strange errors
            setsockopt(childSoket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

            // Receive message
            addrlen = sizeof(addr);
            n = recv(childSoket, buffer, TCP_BUFFER_SIZE, 0);
            if (n == -1)
            {
                perror("(TCP)  Error receiving message");
                exit(EXIT_FAILURE);
            }

            // Print received message
            printf("(TCP) Received: %s", buffer);

            // Check if the request is to send a file
            if (strncmp(buffer, "OPA", 3) == 0)
            {
                printf("(TCP) Receiving file\n");
                // Store metadata
                char metadata[TCP_BUFFER_SIZE];

                // Clear buffer and copy data
                memset(metadata, 0, TCP_BUFFER_SIZE);
                memcpy(metadata, buffer, TCP_BUFFER_SIZE);

                // Create temporary file to read from socket
                std::ofstream tempFile(TMP_FILE, std::ios::binary);
                if (!tempFile)
                {
                    perror("(TCP) Error creating temporary file");
                    exit(EXIT_FAILURE);
                }

                // Write the data into the file (Everything after the last " ")
                char *data = strrchr(buffer, ' ') + 1;
                tempFile.write(data, n - (data - buffer));
                while ((n = recv(childSoket, buffer, TCP_BUFFER_SIZE, 0)) > 0)
                {
                    tempFile.write(buffer, n);
                    memset(buffer, 0 ,TCP_BUFFER_SIZE);
                }

                if (n < 0) // Check if the loop exited because of an error
                {
                    perror("(TCP) Error receiving file");
                    exit(EXIT_FAILURE);
                }

                tempFile.close();

                // Restore metadata
                memcpy(buffer, metadata, TCP_BUFFER_SIZE);         
            }

            // Process the request
            Command status = handleRequest(buffer);
            if (status == Command::COMMAND_NOT_FOUND)
            {
                perror("(TCP) Command not found");
                exit(EXIT_FAILURE);
            }
            printf("(TCP) Sending: %s\n", buffer);
            n = send(childSoket, buffer, n, 0);
            if (n == -1)
            {
                perror("(TCP) Error sending message");
                exit(EXIT_FAILURE);
            }
            
            close(childSoket);
            exit(EXIT_SUCCESS);
        }

        close(childSoket); // Close the child socket for parent
    }

    close(parentSocket);
    freeaddrinfo(res);
    return NULL;
}

int main(int argc, char *argv[])
{
    int ASport = -1;
    char ASportStr[5];
    int GN = GROUP_NUMBER;

    bool Verbose = false;

    int errcode;

    // Parse arguments
    errcode = parseArgs(argc, argv, &ASport, &GN, &Verbose);
    if (errcode == -1)
    {
        perror("(SERVER) Error parsing arguments");
        exit(EXIT_FAILURE);
    }

    // Default settings
    if (ASport == -1)
        ASport = PORT + GN;           // default port
    sprintf(ASportStr, "%d", ASport); // convert port to string

    if (Verbose)
        printf("(SERVER) ASport = %s\n", ASportStr);

    // Create child processes for UDP and TCP
    pid_t udp, tcp;

    udp = fork();
    if (udp == -1)
    {
        perror("(SERVER) Error creating UDP child process");
        exit(EXIT_FAILURE);
    }
    else if (udp == 0)
    {
        // Child process for UDP
        handleUDP(ASportStr);
        exit(EXIT_SUCCESS);
    }

    tcp = fork();
    if (tcp == -1)
    {
        perror("(SERVER) Error creating TCP child process");
        exit(EXIT_FAILURE);
    }
    else if (tcp == 0)
    {
        // Child process for TCP
        handleTCP(ASportStr);
        exit(EXIT_SUCCESS);
    }

    // Wait for child process to finish
    int status;
    waitpid(udp, &status, 0);
    waitpid(tcp, &status, 0);

    return EXIT_SUCCESS;
}