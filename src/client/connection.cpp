#include "connection.hpp"

char *ASIP = strdup("localhost");
int ASport = PORT;
char ASportStr[6]; // TODO check if this is the right size

int fd_udp, fd_tcp, errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints_udp, hints_tcp, *res_udp, *res_tcp;
struct sockaddr_in addr;
char buffer[BUFFER_SIZE];

void initializer(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "n:p:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            ASIP = strdup(optarg);
            break;
        case 'p':
            ASport = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-n ASIP] [-p ASport]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    sprintf(ASportStr, "%d", ASport); // convert port to string
}

void send_udp()
{
    fd_udp = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    memset(&hints_udp, 0, sizeof hints_udp);
    hints_udp.ai_family = AF_INET;      // IPv4
    hints_udp.ai_socktype = SOCK_DGRAM; // UDP socket

    errcode = getaddrinfo(ASIP, ASportStr, &hints_udp, &res_udp);
    if (errcode != 0) /*error*/
        exit(1);

    n = sendto(fd_udp, buffer, strlen(buffer), 0, res_udp->ai_addr, res_udp->ai_addrlen);
    if (n == -1) /*error*/
        exit(1);

    memset(&buffer, 0, sizeof buffer);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    n = recvfrom(fd_udp, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    close(fd_udp);
}

void send_tcp()
{
    fd_tcp = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    memset(&hints_tcp, 0, sizeof hints_tcp);
    hints_tcp.ai_family = AF_INET;       // IPv4
    hints_tcp.ai_socktype = SOCK_STREAM; // TCP socket

    errcode = getaddrinfo(ASIP, ASportStr, &hints_tcp, &res_tcp);
    if (errcode != 0) /*error*/
        exit(1);

    n = connect(fd_tcp, res_tcp->ai_addr, res_tcp->ai_addrlen);
    if (n == -1) /*error*/
        exit(1);

    n = write(fd_tcp, buffer, strlen(buffer));
    if (n == -1) /*error*/
        exit(1);

    memset(&buffer, 0, sizeof(buffer));

    n = read(fd_tcp, buffer, BUFFER_SIZE);
    if (n == -1) /*error*/
        exit(1);

    char *data = buffer;
    for (int i = 0; i < 4; i++)
    {
        data = strchr(data, ' ');
        if (data == NULL)
        {
            close(fd_tcp);
            return;
        }
        data++;
    }

    std::ofstream tempFile(TEMP_PATH, std::ios::binary);
    if (!tempFile)
    {
        perror("Error creating temporary file");
        exit(1);
    }

    tempFile.write(data, n - (data - buffer));
    char file[BUFFER_SIZE];
    while (n>0)
    {
        n = read(fd_tcp, file, BUFFER_SIZE);
        if (n == -1) /*error*/
            exit(1);
        tempFile.write(file, n);
    }
    tempFile.close();
    close(fd_tcp);
}

void send_tcp(std::string filename)
{
    buffer[strlen(buffer) - 1] = ' ';

    fd_tcp = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    memset(&hints_tcp, 0, sizeof hints_tcp);
    hints_tcp.ai_family = AF_INET;       // IPv4
    hints_tcp.ai_socktype = SOCK_STREAM; // TCP socket

    errcode = getaddrinfo(ASIP, ASportStr, &hints_tcp, &res_tcp);
    if (errcode != 0) /*error*/
        exit(1);

    n = connect(fd_tcp, res_tcp->ai_addr, res_tcp->ai_addrlen);
    if (n == -1) /*error*/
        exit(1);

    write(fd_tcp, buffer, strlen(buffer));

    // Open the file
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        printf("Error opening file %s\n", filename.c_str());
        exit(1);
    }

    // Read and send the file in chunks
    while (!file.eof())
    {
        file.read(buffer, BUFFER_SIZE);
        std::streamsize dataSize = file.gcount();

        // Send the file data via TCP
        n = write(fd_tcp, buffer, dataSize);
        if (n == -1) /*error*/
            exit(1);
    }

    write(fd_tcp, "\n", 1);

    memset(buffer, 0, sizeof(buffer));

    n = read(fd_tcp, buffer, BUFFER_SIZE);
    if (n == -1) /*error*/
        exit(1);

    close(fd_tcp);
}

void close_socket()
{
    close(fd_tcp);
    close(fd_udp);
}