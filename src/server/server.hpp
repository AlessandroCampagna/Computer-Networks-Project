#ifndef SERVER_H
#define SERVER_H

#include "handler.hpp"
#include "sockets.hpp"

#include <unistd.h>
#include <stdlib.h>

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