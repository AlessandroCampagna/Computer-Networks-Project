#ifndef HANDLER_H
#define HANDLER_H

#include <stdlib.h>

#include <cstring>
#include <functional>
#include <unordered_map>

enum Command
{
    COMMAND_NOT_FOUND,
    COMAND_COMPLETED
};

Command handle_request(char *buffer);

#endif