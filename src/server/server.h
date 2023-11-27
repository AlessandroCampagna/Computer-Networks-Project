#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string>
#include <cstring>

// Handler
#include <functional>
#include <unordered_map>

#define BUFFER_SIZE 128
#define PORT 58000
#define GROUP_NUMBER 17
#define BACKLOG 5
#define TIME_OUT 5

enum ConnectionType {
    UDP,
    TCP,
    EXIT,
    INVALID
};

struct Connection {
    ConnectionType type;
    int socket;
};

ConnectionType handle_request(char *buffer);

#endif