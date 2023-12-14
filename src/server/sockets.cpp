#include "sockets.hpp"

int openUDP(char *port)
{

    int error;

    struct addrinfo hints, *res;
    int udp_socket;

    // Create UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (udp_socket == -1)
    {
        perror("(SOCKETS) Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    // Define address structure for UDP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    // Get UDP address info
    error = getaddrinfo(NULL, port, &hints, &res);
    if (error == -1)
    {
        perror("(SOCKETS) Error getting UDP address info");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket
    error = bind(udp_socket, res->ai_addr, res->ai_addrlen);
    if (error == -1)
    {
        perror("(SOCKETS) Error binding UDP socket");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);
    return udp_socket;
}

int openTCP(char *port)
{
    int error;

    struct addrinfo hints, *res;
    int tcp_socket;

    // Create TCP socket for incoming connections
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (tcp_socket == -1)
    {
        perror("(SOCKETS) Error creating TCP socket");
        exit(EXIT_FAILURE);
    }

    // Define address structure for TCP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;

    // Get TCP address info
    error = getaddrinfo(NULL, port, &hints, &res);
    if (error == -1)
    {
        perror("(SOCKETS) Error getting TCP address info");
        exit(EXIT_FAILURE);
    }

    // Bind TCP socket
    error = bind(tcp_socket, res->ai_addr, res->ai_addrlen);
    if (error == -1)
    {
        perror("(SOCKETS) Error binding TCP socket");
        exit(EXIT_FAILURE);
    }

    return tcp_socket;
}