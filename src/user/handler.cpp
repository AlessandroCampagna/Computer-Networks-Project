#include "user.h"

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<ConnectionType(Tokens*)>;

Tokens parse_buffer(char *buffer);
void diparse_buffer(char* buffer, Tokens* tokens);

ConnectionType login(Tokens*);
ConnectionType logout(Tokens*);
ConnectionType unregister(Tokens*);
ConnectionType myauctions(Tokens*);
ConnectionType exituser(Tokens*);

std::string uid;
std::string password;

const std::unordered_map<std::string, CommandFunction> command_map = {
    {"login", login},
    {"logout", logout},
    {"unregister", unregister},
    {"myauctions", myauctions},
    {"exit", exituser}
};

// Define the user_command function
ConnectionType user_command(char* buffer) {
    
    Tokens tokens = parse_buffer(buffer);
    auto it = command_map.find(tokens[0]);

    // If the command is found, execute the associated function and return its value
    if (it != command_map.end()) {
        ConnectionType result = it->second(&tokens);
        diparse_buffer(buffer, &tokens);
        return result;
    }

    // If the command is not found, return an error value
    return ConnectionType::INVALID;
}

Tokens parse_buffer(char *buffer) {

    // Make the buffer a cpp string
    std::string str(buffer);
    
    // Split the string into tokens
    std::string delimiter = " ";
    Tokens tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter[0])) {
        tokens.push_back(token);
    }

    return tokens;
}

void diparse_buffer(char* buffer, Tokens* tokens) {
    std::string result = "";
    for (auto word :  *tokens) {
        result += word + " ";
    }
    result.pop_back(); // remove the last space
    std::strcpy(buffer, result.c_str());
}

// Define the functions for each command
ConnectionType login(Tokens* tokens) {
    (*tokens)[0] = "LIN";
    std::string uid = (*tokens)[1];
    std::string password = (*tokens)[2];
    return ConnectionType::UDP;
}

std::string login_response(char* buffer) {
    Tokens tokens = parse_buffer(buffer);
    if (tokens[1] == "OK"){
        return 0;
    }
}

ConnectionType logout(Tokens* tokens) {
    (*tokens)[0] = "LOU";
    std::string uid = (*tokens)[1];
    std::string password = (*tokens)[2];
    return ConnectionType::UDP;
}

ConnectionType unregister(Tokens* tokens) {
    (*tokens)[0] = "UNR";
    std::string uid = (*tokens)[1];
    std::string password = (*tokens)[2];
    return ConnectionType::UDP;
}
ConnectionType myauctions(Tokens* tokens) {
    (*tokens)[0] = "LMA";
    std::string uid = (*tokens)[1];
    std::string password = (*tokens)[2];
    return ConnectionType::UDP;
}

ConnectionType exituser(Tokens* tokens) {
    return ConnectionType::EXIT;
}


