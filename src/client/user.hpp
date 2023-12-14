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

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<void(Tokens *)>;

extern const std::unordered_map<std::string, CommandFunction> command_map;

void send_udp(Tokens *tokens);
void send_tcp(Tokens *tokens);
void send_tcp(Tokens *tokens, std::string filename);

#endif