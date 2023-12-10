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
#include <regex>
#include <filesystem>
#include <fstream>

#define PORT 58017
#define BUFFER_SIZE 6010

#define ASSETS_PATH "../../ASSETS/"

extern std::string uid;
extern std::string password;
extern bool logged;

enum ConnectionType
{
    UDP,
    TCP,
    EXIT,
    INVALID
};

ConnectionType handle_command(char *buffer);
void handle_response(char *buffer);

#endif