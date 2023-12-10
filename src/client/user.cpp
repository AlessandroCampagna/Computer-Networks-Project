#include "user.hpp"

void initializer(int argc, char *argv[], char *&ASIP, int &ASport, char *ASportStr);
Tokens parse_buffer(char *buffer);
void diparse_buffer(char *buffer, Tokens *tokens);

int main(int argc, char *argv[])
{
    char *ASIP = NULL;
    int ASport = -1;
    char ASportStr[6]; // TODO check if this is the right size

    int fd_udp, fd_tcp, errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints_udp, hints_tcp, *res_udp, *res_tcp;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    initializer(argc, argv, ASIP, ASport, ASportStr);

    fd_udp = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    memset(&hints_udp, 0, sizeof hints_udp);
    hints_udp.ai_family = AF_INET;      // IPv4
    hints_udp.ai_socktype = SOCK_DGRAM; // UDP socket
    errcode = getaddrinfo(ASIP, ASportStr, &hints_udp, &res_udp);
    if (errcode != 0) /*error*/
        exit(1);

    while (true)
    {

        fgets(buffer, sizeof(buffer), stdin);

        buffer[strlen(buffer) - 1] = '\0';
        Tokens tokens = parse_buffer(buffer);
        ConnectionType connectionType = handle_command(&tokens);

        if (connectionType == UDP)
        {
            diparse_buffer(buffer, &tokens);
            n = sendto(fd_udp, buffer, strlen(buffer), 0, res_udp->ai_addr, res_udp->ai_addrlen);
            if (n == -1) /*error*/
                exit(1);

            memset(&buffer, 0, sizeof buffer);
            addrlen = sizeof(addr);

            n = recvfrom(fd_udp, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addrlen);
            if (n == -1) /*error*/
                exit(1);

            buffer[strlen(buffer) - 1] = '\0';
            Tokens tokens = parse_buffer(buffer);
            handle_response(&tokens);
        }
        else if (connectionType == TCP)
        {

            printf("socket\n");
            fd_tcp = socket(AF_INET, SOCK_STREAM, 0); // TCP socket

            memset(&hints_tcp, 0, sizeof hints_tcp);
            hints_tcp.ai_family = AF_INET;       // IPv4
            hints_tcp.ai_socktype = SOCK_STREAM; // TCP socket

            printf("getaddrinfo\n");
            errcode = getaddrinfo(ASIP, ASportStr, &hints_tcp, &res_tcp);
            if (errcode != 0) /*error*/
                exit(1);

            printf("connect\n");
            n = connect(fd_tcp, res_tcp->ai_addr, res_tcp->ai_addrlen);
            if (n == -1) /*error*/
                exit(1);

            printf("send: %s\n", buffer);
            diparse_buffer(buffer, &tokens);
            n = send(fd_tcp, buffer, strlen(buffer), 0);
            if (n == -1) /*error*/
                exit(1);
            memset(&buffer, 0, sizeof(buffer));

            if (tokens[0] == "OPA")
            {
                buffer[strlen(buffer) - 1] = '\0';
                std::ifstream file(ASSETS_PATH + tokens[6], std::ios::binary);
                int bytesRead;
                while ((bytesRead = file.readsome(buffer, sizeof(buffer))) > 0)
                {
                    printf("send: %d\n", buffer);
                    n = send(fd_tcp, buffer, bytesRead, 0);
                    if (n == -1) /*error*/
                        exit(1);
                }
                file.close();

                n = send(fd_tcp, "\n", strlen("\n"), 0);
            }
            memset(&buffer, 0, sizeof(buffer));

            n = read(fd_tcp, buffer, BUFFER_SIZE);
            printf("read: %s\n", buffer);
            if (n == -1) /*error*/
                exit(1);
        }
        else if (connectionType == EXIT)
        {
            if (!logged)
                exit(1);
            else
                printf("You must logout before exiting\n");
        }
        else
        {
            printf("Invalid Command\n");
        }
    }

    free(ASIP);
    return 0;
}

void initializer(int argc, char *argv[], char *&ASIP, int &ASport, char *ASportStr)
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

    // Default settings
    if (ASIP == NULL)
        ASIP = strdup("localhost"); // default IP
    if (ASport == -1)
        ASport = PORT;                // default port
    sprintf(ASportStr, "%d", ASport); // convert port to string
}

Tokens parse_buffer(char *buffer)
{

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

void diparse_buffer(char *buffer, Tokens *tokens)
{
    std::string result = "";
    for (auto word : *tokens)
    {
        result += word + " ";
    }
    result.pop_back();
    result += "\n";
    std::strcpy(buffer, result.c_str());
}