#include "server.hpp"

// TODO: Handle SIG_PIPE and zombie processes
//       Handle CTRL+C (SIGINT) and CTRL+Z (SIGTSTP) exit with grace

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

void handleUDP(char *ASportStr)
{

    int messageSize;
    char buffer[UDP_BUFFER_SIZE];

    int UDPsocket = openUDP(ASportStr);
    struct sockaddr_in addr;
    socklen_t addrlen;

    while (true)
    {
        // Clear buffer
        memset(buffer, 0, UDP_BUFFER_SIZE);

        // Receive message
        addrlen = sizeof(addr);
        messageSize = recvfrom(UDPsocket, buffer, UDP_BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addrlen);
        if (messageSize == -1)
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
        messageSize = sendto(UDPsocket, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, addrlen);
        if (messageSize == -1)
        {
            perror("(UDP) Error sending message");
            exit(EXIT_FAILURE);
        }
    }

    close(UDPsocket);
}

void handleTCPchild(int childSocket)
{

    int messageSize;
    char buffer[TCP_BUFFER_SIZE];

    fd_set read_fds;
    struct timeval timeout;

    // Initialize the file descriptor set
    FD_ZERO(&read_fds);
    FD_SET(childSocket, &read_fds);

    // Set timeout to 5 seconds
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // Use select to wait for data to be available.
    int result = select(childSocket + 1, &read_fds, NULL, NULL, &timeout);

    if (result == -1)
    {
        // An error occurred with select
        perror("(TCP) Error with select function");
        exit(EXIT_FAILURE);
    }
    else if (result == 0)
    {
        // The timeout was reached without any data being available
        printf("(TCP) Timeout reached without any data\n");
        return;
    }

    // Clear buffer
    memset(buffer, 0, TCP_BUFFER_SIZE);

    messageSize = recv(childSocket, buffer, TCP_BUFFER_SIZE, 0);
    if (messageSize == -1)
    {
        perror("(TCP) Error receiving message");
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
        std::ofstream tempFile(TEMP_PATH, std::ios::binary);
        if (!tempFile)
        {
            perror("(TCP) Error creating temporary file");
            exit(EXIT_FAILURE);
        }
        // Write the data into the file (Everything after the last " ")
        char *data = strrchr(buffer, ' ') + 1;
        tempFile.write(data, messageSize - (data - buffer));
        while ((messageSize = recv(childSocket, buffer, TCP_BUFFER_SIZE, 0)) > 0)
        {
            tempFile.write(buffer, messageSize);
            memset(buffer, 0, TCP_BUFFER_SIZE);
        }
        if (messageSize < 0) // Check if the loop exited because of an error
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
    messageSize = send(childSocket, buffer, messageSize, 0);
    if (messageSize == -1)
    {
        perror("(TCP) Error sending message");
        exit(EXIT_FAILURE);
    }

    close(childSocket);
    exit(EXIT_SUCCESS);
}

void handleTCP(char *ASportStr)
{

    int parentSocket = openTCP(ASportStr);
    struct sockaddr_in addr;
    socklen_t addrlen;

    while (true)
    {
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
            close(parentSocket);
            handleTCPchild(childSoket);
        }

        close(childSoket);
    }

    close(parentSocket);
}

int main(int argc, char *argv[])
{
    int ASport = -1;
    char ASportStr[5];
    int GN = GROUP_NUMBER;
    bool Verbose = false;

    int error;

    // Parse arguments
    error = parseArgs(argc, argv, &ASport, &GN, &Verbose);
    if (error == -1)
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