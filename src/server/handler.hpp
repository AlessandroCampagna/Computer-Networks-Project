#ifndef HANDLER_H
#define HANDLER_H

#include <stdlib.h>

#include <cstring>
#include <functional>
#include <unordered_map>

#include "data.hpp"

enum Command
{
    COMMAND_NOT_FOUND,
    COMAND_COMPLETED,
    COMMAND_RECIEVE_IMAGE,
    COMMAND_SEND_IMAGE,
};

Command handleRequest(char *buffer);

#endif