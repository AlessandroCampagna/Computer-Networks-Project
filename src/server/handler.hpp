#ifndef HANDLER_H
#define HANDLER_H

#include <cstring>
#include <functional>

#include "data.hpp"

enum Command
{
    COMMAND_NOT_FOUND,
    COMAND_COMPLETED
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