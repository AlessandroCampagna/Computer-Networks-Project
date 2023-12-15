#include "connection.hpp"

int UDPsocket;
int TCPsocket;
int TCPchildSocket;

static void TCPChild(int childSocket, struct sockaddr_in addr);
static void TCPFile(int childSocket, char *buffer, int messageSize);

void closeSockets()
{
    if (UDPsocket != -1) close(UDPsocket);
    if (TCPsocket != -1) close(TCPsocket);
    if (TCPchildSocket != -1) close(TCPchildSocket);
}

void UDPConnection(char *port)
{

    int messageSize;
    char buffer[UDP_BUFFER_SIZE];

    struct sockaddr_in addr;
    socklen_t addrlen;

    UDPsocket = openUDP(port);

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
        printf("(UDP) From %s:%d received %s", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);

        // Process request
        Command status = handleRequest(buffer);

        if (status == Command::COMMAND_NOT_FOUND)
        {
            perror("(UDP) Command not found");
            exit(EXIT_FAILURE);
        }

        // Print message to send
        printf("(UDP) Sending to %s:%d message %s", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);

        messageSize = sendto(UDPsocket, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, addrlen);
        if (messageSize == -1)
        {
            perror("(UDP) Error sending message");
            exit(EXIT_FAILURE);
        }
    }

    close(UDPsocket);
}

void TCPConnection(char *port)
{

    struct sockaddr_in addr;
    socklen_t addrlen;

    TCPsocket = openTCP(port);

    while (true)
    {
        // Listen for connections
        if (listen(TCPsocket, BACKLOG) == -1)
        {
            perror("(TCP) Error listening on socket");
            exit(EXIT_FAILURE);
        }

        // Accept a connection and create a socket for it
        addrlen = sizeof(addr);
        int TCPchildSocket = accept(TCPsocket, (struct sockaddr *)&addr, &addrlen);
        if (TCPchildSocket == -1)
        {
            perror("(TCP) Error accepting connection");
            exit(EXIT_FAILURE);
        }

        // Fork a new process to handle the request
        printf("(TCP) Forking a new process for incoming request\n");
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("(TCP) Error creating a new process");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // Child process to handle request
        {
            close(TCPsocket);
            TCPChild(TCPchildSocket, addr);
        }

        close(TCPchildSocket);
    }

    close(TCPsocket);
}

void TCPChild(int childSocket, struct sockaddr_in addr)
{
    int messageSize;
    char buffer[TCP_BUFFER_SIZE];

    // Clear buffer
    memset(buffer, 0, TCP_BUFFER_SIZE);

    messageSize = recv(childSocket, buffer, TCP_BUFFER_SIZE, 0);
    if (messageSize == -1)
    {
        perror("(TCP) Error receiving message");
        exit(EXIT_FAILURE);
    }

    // Print received message
    printf("(TCP) Sending to %s:%d message %s", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);

    // Handle file transfer
    if (strncmp(buffer, "OPA", 3) == 0)
    {
        TCPFile(childSocket, buffer, messageSize);
    }

    // Process the request
    Command status = handleRequest(buffer);
    if (status == Command::COMMAND_NOT_FOUND)
    {
        perror("(TCP) Command not found");
        exit(EXIT_FAILURE);
    }

    printf("(TCP) Sending to %s:%d message %s", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);

    messageSize = send(childSocket, buffer, messageSize, 0);
    if (messageSize == -1)
    {
        perror("(TCP) Error sending message");
        exit(EXIT_FAILURE);
    }

    close(childSocket);
    exit(EXIT_SUCCESS);
}

void TCPFile(int childSocket, char *buffer, int messageSize)
{
    printf("\n(TCP) Receiving file \n");

    // Store metadata
    char metadata[TCP_BUFFER_SIZE];
    int fileSize;

    // Clear buffer and copy data
    memset(metadata, 0, TCP_BUFFER_SIZE);
    memcpy(metadata, buffer, TCP_BUFFER_SIZE);

    // Parse metadata into tokens (using c++)
    // Make the buffer a cpp string
    std::string str(metadata);

    // Split the string into tokens
    std::string delimiter = " ";
    Tokens tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter[0]))
    {
        tokens.push_back(token);
    }

    // Get file size
    fileSize = atoi(tokens[7].c_str());

    // Create temporary file to read from socket
    std::ofstream tempFile(TEMP_PATH, std::ios::binary);
    if (!tempFile)
    {
        perror("(TCP) Error creating temporary file");
        exit(EXIT_FAILURE);
    }

    // Write the data into the file (Everything after first 8 tokens in buffer)
    char *data = buffer;
    for (int i = 0; i < 8; i++)
    {
        data = strchr(data, ' ');
        if (data == NULL)
        {
            perror("(TCP) Error parsing metadata to find file data");
            exit(EXIT_FAILURE);
        }
        data++;
    }
    tempFile.write(data, messageSize - (data - buffer));
    fileSize -= messageSize - (data - buffer);

    memset(buffer, 0, TCP_BUFFER_SIZE);
    printf("(TCP) Entering file data loop\n");
    while (fileSize > 0)
    {
        messageSize = recv(childSocket, buffer, TCP_BUFFER_SIZE, 0);
        if (messageSize == -1)
        {
            perror("(TCP) Error receiving file");
            exit(EXIT_FAILURE);
        }
        tempFile.write(buffer, messageSize);
        fileSize -= messageSize;
        memset(buffer, 0, TCP_BUFFER_SIZE);
    }
    {
        tempFile.write(buffer, messageSize);
        memset(buffer, 0, TCP_BUFFER_SIZE);
    }
    if (messageSize < 0) // Check if the loop exited because of an error
    {
        perror("(TCP) Error receiving file");
        exit(EXIT_FAILURE);
    }
    printf("(TCP) Exiting file data loop and closing file\n");
    tempFile.close();

    // Restore metadata without file data
    memset(buffer, 0, TCP_BUFFER_SIZE);
    memcpy(buffer, metadata, TCP_BUFFER_SIZE);
}