#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <string>
#include <cstdio>
#include <unordered_map>
#include <functional>

#define BUFFER_SIZE 128
#define PORT 58000
#define GROUP_NUMBER 17

enum ConnectionType {
    UDP,
    TCP,
    EXIT,
    INVALID
};

std::vector<std::string> parse_buffer(char *buffer);
int handle_request(char *buffer);
int handle_login(std::vector<std::string> token);
ConnectionType login(std::vector<std::string> token);
ConnectionType logout(std::vector<std::string> token);
ConnectionType unregister(std::vector<std::string> token);
ConnectionType myauctions(std::vector<std::string> token);

#endif