#include "user.hpp"

int main(int argc, char *argv[])
{
    initializer(argc, argv);
    while (true)
    {
        fgets(buffer, sizeof(buffer), stdin);
        handle_buffer();
    }
    return 0;
}