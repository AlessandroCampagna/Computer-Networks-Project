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

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<Command(Tokens *)>;

// UDP commands
Command login(Tokens *);
Command logout(Tokens *);
Command unregister(Tokens *);
Command myauctions(Tokens *);
// TCP commands
Command openAuction(Tokens *);
Command closeAuction(Tokens *);
Command sendAsset(Tokens *);
Command placeBid(Tokens *);

Command handleRequest(char *buffer);

#endif