#include "handler.hpp"

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<Command(Tokens *)>;

// UDP commands
Command login(Tokens *);
Command logout(Tokens *);
Command unregister(Tokens *);
Command myauctions(Tokens *);
Command exituser(Tokens *);
// TCP commands
Command openAuction(Tokens *);
Command closeAuction(Tokens *);
Command sendAsset(Tokens *);
Command placeBid(Tokens *);

const std::unordered_map<std::string, CommandFunction> command_map = {
    // UDP commands
    {"LIN", login},
    {"LOU", logout},
    {"UNR", unregister},
    {"LMA", myauctions},
    // TCP commands
    {"OPA", openAuction},
    {"CLS", closeAuction},
    {"SAS", sendAsset},
    {"BID", placeBid}};

Tokens parseBuffer(char *buffer)
{

    // Make the buffer a cpp string
    std::string str(buffer);

    str.erase(str.size() - 1); // Remove the last character (newline)

    // Split the string into tokens
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

void diparseBuffer(char *buffer, Tokens *tokens)
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

Command handleRequest(char *buffer)
{

    Tokens tokens = parseBuffer(buffer);
    auto it = command_map.find(tokens[0]);

    // If the command is found, execute the associated function and return its value
    if (it != command_map.end())
    {
        Command result = it->second(&tokens);
        diparseBuffer(buffer, &tokens);
        return result;
    }

    // If the command is not found, return an error value
    return Command::COMMAND_NOT_FOUND;
}

Command login(Tokens *token)
{
    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];

    // Check if the user exists in the database
    if (isUser(uid) == false || isPassword(uid, "") == true)
    {
        createUser(uid, password);
        loginUser(uid);
        response.push_back("RLI");
        response.push_back("REG");
    }
    else
    {
        if (isPassword(uid, password) == false)
        {
            response.push_back("RLI");
            response.push_back("NOK");
        }
        else
        {
            loginUser(uid);
            response.push_back("RLI");
            response.push_back("OK");
        }
    }

    // Replace token with response
    *token = response;

    return Command::COMAND_COMPLETED;
}

Command logout(Tokens *token)
{
    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];

    // Check if the user dose not exists in the database
    if (isUser(uid) == false)
    {
        response.push_back("RLO");
        response.push_back("UNR");
    }
    else if (isLogin(uid) == false)
    {
        response.push_back("RLO");
        response.push_back("NOK");
    }
    else
    {
        logoutUser(uid);
        response.push_back("RLO");
        response.push_back("OK");
    }

    *token = response;

    return Command::COMAND_COMPLETED;
}

Command unregister(Tokens *token)
{
    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];

    // Check if the user dose not exists in the database
    if (isUser(uid) == false)
    {
        response.push_back("RUR");
        response.push_back("UNR");
    }
    else if (isPassword(uid, password) == false)
    {
        response.push_back("RUR");
        response.push_back("NOK");
    }
    else
    {
        removeUser(uid);
        response.push_back("RUR");
        response.push_back("OK");
    }

    *token = response;
    return Command::COMAND_COMPLETED;
}

Command myauctions(Tokens *token)
{
    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];

    if (isLogin(uid) == false)
    {
        response.push_back("RMA");
        response.push_back("NLG");
    }
    else if (areAuctions(uid) == false)
    {
        response.push_back("RMA");
        response.push_back("NOK");
    }
    else
    {
        response.push_back("RMA");
        response.push_back("OK");
        for (auto auction : getAuctions(uid))
        {
            response.push_back(auction);
        }
    }

    *token = response;
    return Command::COMAND_COMPLETED;
}

Command openAuction(Tokens *token){
    return Command::COMAND_COMPLETED;
}

Command closeAuction(Tokens *token){
    return Command::COMAND_COMPLETED;
}

Command sendAsset(Tokens *token){
    return Command::COMAND_COMPLETED;
}

Command placeBid(Tokens *token){
    return Command::COMAND_COMPLETED;
}