#include "server.hpp"

void signalHandler(int signum)
{
    if (signum == SIGPIPE)
    {
        // Handle broken pipe signal
        std::cout << "(SERVER) Received SIGPIPE, ignoring it" << std::endl;
    }
    else if (signum == SIGINT)
    {
        // Handle interrupt signal (CTRL+C)
        std::cout << "(SERVER) Received SIGINT, exiting" << std::endl;
        closeSockets();
        exit(EXIT_SUCCESS);
    }
    else if (signum == SIGTSTP)
    {
        // Handle stop signal (CTRL+Z)
        std::cout << "(SERVER) Received SIGTSTP, exiting" << std::endl;
        closeSockets();
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    char port[PORT_SIZE];
    bool verbose = false;

    // Initialize signal handler
    signal(SIGPIPE, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);

    // Load default port
    sprintf(port, "%d", PORT + GROUP);

    int opt;
    while ((opt = getopt(argc, argv, "p:v")) != -1)
    {
        switch (opt)
        {
        case 'p':
            memccpy(port, optarg, 0, PORT_SIZE);
            break;
        case 'v':
            // TODO: Print client IP and port
            verbose = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-p <port>] [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Disable printing when not in verbose mode
    if (!verbose)
        freopen("/dev/null", "w", stdout);

    // Create child processes for UDP and TCP
    pid_t udp, tcp;

    udp = fork();
    if (udp == -1)
    {
        perror("(SERVER) Error creating UDP child process");
        exit(EXIT_FAILURE);
    }
    else if (udp == 0)
    {
        UDPConnection(port);
        exit(EXIT_SUCCESS);
    }

    tcp = fork();
    if (tcp == -1)
    {
        perror("(SERVER) Error creating TCP child process");
        exit(EXIT_FAILURE);
    }
    else if (tcp == 0)
    {
        TCPConnection(port);
        exit(EXIT_SUCCESS);
    }

    // Wait for child process to finish
    int status;
    waitpid(udp, &status, 0);
    waitpid(tcp, &status, 0);
    printf("(SERVER) Exiting\n");
    return EXIT_SUCCESS;
}