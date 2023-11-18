#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <string>
#include <cstdio>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <fstream>
#include <cstring>

#define BUFFER_SIZE 128
#define PORT 58000
#define GROUP_NUMBER 17

#define ASDIR_PATH "../../ASDIR/"

enum ConnectionType {
    UDP,
    TCP,
    EXIT,
    INVALID
};

ConnectionType handle_request(char *buffer);

#endif