#include "user.hpp"

void initializer(int argc, char *argv[]);
void handle_buffer(Tokens *tokens);
Tokens parse_buffer();
void diparse_buffer(Tokens *tokens);

void send_udp(Tokens *tokens);
void send_tcp(Tokens *tokens);
void send_file(Tokens *tokens);

char *ASIP = strdup("localhost");
int ASport = PORT;
char ASportStr[6]; // TODO check if this is the right size

int fd_udp, fd_tcp, errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints_udp, hints_tcp, *res_udp, *res_tcp;
struct sockaddr_in addr;
char buffer[BUFFER_SIZE];

int main(int argc, char *argv[])
{
    initializer(argc, argv);
    while (true)
    {
        memset(&hints_tcp, 0, sizeof hints_tcp);
        fgets(buffer, sizeof(buffer), stdin);
        Tokens tokens = parse_buffer();
        handle_buffer(&tokens);
    }

    free(ASIP);
    return 0;
}

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

    fd_udp = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket

    memset(&hints_udp, 0, sizeof hints_udp);
    hints_udp.ai_family = AF_INET;      // IPv4
    hints_udp.ai_socktype = SOCK_DGRAM; // UDP socket

    errcode = getaddrinfo(ASIP, ASportStr, &hints_udp, &res_udp);
    if (errcode != 0) /*error*/
        exit(1);
}

void handle_buffer(Tokens *tokens)
{
    auto it = command_map.find((*tokens)[0]);
    if (it != command_map.end())
    {
        it->second(tokens);
    }
    else
    {
        printf("Invalid Command\n");
    }
}

void send_udp(Tokens *tokens)
{
    diparse_buffer(tokens);

    n = sendto(fd_udp, buffer, strlen(buffer), 0, res_udp->ai_addr, res_udp->ai_addrlen);
    if (n == -1) /*error*/
        exit(1);

    memset(&buffer, 0, sizeof buffer);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    n = recvfrom(fd_udp, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&res_udp, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    *tokens = parse_buffer();
    handle_buffer(tokens);
}

void send_tcp(Tokens *tokens)
{
    diparse_buffer(tokens);

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

    *tokens = parse_buffer();
    handle_buffer(tokens);
}

void send_tcp(Tokens *tokens, std::string filename)
{
}

Tokens parse_buffer()
{
    buffer[strlen(buffer) - 1] = '\0';
    std::string str(buffer);

    std::string delimiter = " ";
    Tokens tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter[0]))
    {
        tokens.push_back(token);
    }

    return tokens;
}

void diparse_buffer(Tokens *tokens)
{
    std::string result = "";
    for (auto word : *tokens)
    {
        result += word + " ";
    }
    result.pop_back(); // remove the last space
    result += "\n";
    std::strcpy(buffer, result.c_str());
}