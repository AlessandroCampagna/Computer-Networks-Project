#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>


#define GN 17
#define PORT 58000 + GN
#define BUFFER_SIZE 6010

#define TEMP_PATH "../../ASSETS/temp"

extern char buffer[BUFFER_SIZE];

void initializer(int argc, char *argv[]);
void send_udp();
void send_tcp();
void send_tcp(std::string);
void send_txt(std::string);
void close_socket();

#endif