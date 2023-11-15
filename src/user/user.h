#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <cstdio>

#define PORT 58000
#define BUFFER_SIZE 128

enum ConnectionType {
    UDP,
    TCP
};
