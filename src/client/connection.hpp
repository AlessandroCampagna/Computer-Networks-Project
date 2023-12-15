#ifdef CONNECTION_H
#define CONNECTION_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define GN 17
#define PORT 58000 + GN
#define BUFFER_SIZE 6010

extern char buffer[BUFFER_SIZE];

#endif