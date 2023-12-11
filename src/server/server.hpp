#ifndef SERVER_H
#define SERVER_H

#include "handler.hpp"

#include <unistd.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <cstdio>
#include <cstring>   


#define TCP_BUFFER_SIZE 6010
#define UDP_BUFFER_SIZE 128
#define PORT 58000
#define GROUP_NUMBER 17
#define BACKLOG 5
#define TIME_OUT 5
#define TMP_FILE "temp"

#endif