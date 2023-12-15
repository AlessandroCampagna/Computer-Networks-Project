#ifndef CONNECTION_H
#define CONNECTION_H

#include "sockets.hpp"
#include "handler.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <iostream>
#include <cstdio>
#include <cstring>

#include "handler.hpp"
#include "sockets.hpp"

#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 6010

#define BACKLOG 5
#define TIME_OUT 5

void closeSockets();

void UDPConnection(char *ASportStr);
void TCPConnection(char *ASportStr);

#endif