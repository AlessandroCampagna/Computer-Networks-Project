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


#define PORT 58017
#define BUFFER_SIZE 128

extern std::string uid;
extern std::string password;
extern bool logged;

enum ConnectionType {
    UDP,
    TCP,
    EXIT,
    INVALID
};

ConnectionType handle_command(char* buffer);
void handle_response(char* buffer);

#endif
