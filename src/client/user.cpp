#include "user.hpp"

int main(int argc, char *argv[])
{
    signal(SIGPIPE, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);

    initializer(argc, argv);
    while (true)
    {
        memset(&buffer, 0, sizeof buffer);
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        handle_buffer();
        printf("\n");
    }
    return 0;
}