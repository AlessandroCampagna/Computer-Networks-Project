#ifndef CONNECTION_H
#define CONNECTION_H

#include "sockets.hpp"
#include "handler.hpp"

#include <iostream>

#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 6010

#define BACKLOG 5
#define TIME_OUT 5

void closeSockets();

void UDPConnection(char *ASportStr);
void TCPConnection(char *ASportStr);

#endif