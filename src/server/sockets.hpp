#ifndef SOCKETS_H
#define SOCKETS_H

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

int openUDP(char *port);
int openTCP(char *port);

#endif