#ifndef USER_H
#define USER_H


#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <cstdio>
#include <unordered_map>
#include <functional>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>


#define PORT 58000
#define BUFFER_SIZE 128

enum ConnectionType {
    UDP,
    TCP,
    EXIT,
    INVALID
};

ConnectionType user_command(char* buffer);

#endif
