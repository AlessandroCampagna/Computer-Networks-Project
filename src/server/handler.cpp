#include "handler.hpp"

// TODO: Implement autentication logic

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

    // Check if the user exists
    if (!isUser(uid) || isPassword(uid, ""))
    {
        createUser(uid, password);
        loginUser(uid);
        response.push_back("RLI");
        response.push_back("REG");
    }
    else
    {
        if (!isPassword(uid, password))
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

    // Check if the user dose not exists
    if (!isUser(uid))
    {
        response.push_back("RLO");
        response.push_back("UNR");
    }
    else if (!isLogin(uid))
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

    // Check if the user dose not exist
    if (!isUser(uid))
    {
        response.push_back("RUR");
        response.push_back("UNR");
    }
    else if (!isPassword(uid, password))
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

    if (!isLogin(uid))
    {
        response.push_back("RMA");
        response.push_back("NLG");
    }
    else if (!areUserAuctions(uid))
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

Command openAuction(Tokens *token)
{

    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];
    std::string name = (*token)[3];
    std::string startValue = (*token)[4];
    std::string timeActive = (*token)[5];
    std::string fileName = (*token)[6];
    std::string fileSize = (*token)[7];
    std::string fileData = (*token)[8];

    if (!isLogin(uid))
    {
        response.push_back("ROA");
        response.push_back("NLG");
        *token = response;
        return Command::COMAND_COMPLETED;
    }

    std::string aid = createAuction(uid, name, startValue, timeActive, fileName, fileSize, fileData);
    if (aid == "") //TODO: implement fs error logic
    {
        response.push_back("ROA");
        response.push_back("NOK");
        *token = response;
        return Command::COMAND_COMPLETED;
    }

    response.push_back("ROA");
    response.push_back("OK");
    response.push_back(aid);
    *token = response;
    return Command::COMAND_COMPLETED;
}

Command closeAuction(Tokens *token)
{
    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];
    std::string aid = (*token)[3]; 

    int error = removeAuction(aid);
    if (error == -1) {
        response.push_back("RCL");
        response.push_back("NOK"); //TODO: Implement real error response
        *token = response;
        return Command::COMAND_COMPLETED;    
    }

    response.push_back("RCL");
    response.push_back("OK");
    *token = response;
    return Command::COMAND_COMPLETED;
}

Command sendAsset(Tokens *token)
{
    //TODO: Implement TCP file trasnfer
    return Command::COMAND_COMPLETED;
}

Command placeBid(Tokens *token)
{
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];
    std::string aid = (*token)[3]; 
    std::string value = (*token)[4];

    int error = createBid(uid, aid, value);
    if (error == -1){ //TODO: Implemnet errors
        response.push_back("RBD");
        response.push_back("NOK");
        *token = response;
        return Command::COMAND_COMPLETED;  
    }

    response.push_back("RBD");
    response.push_back("OK");
    *token = response;
    return Command::COMAND_COMPLETED;
}