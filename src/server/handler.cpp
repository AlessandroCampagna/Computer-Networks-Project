#include "server.h"

using CommandFunction = std::function<ConnectionType(std::vector<std::string>*)>;

ConnectionType login(std::vector<std::string>*);
ConnectionType logout(std::vector<std::string>*);
ConnectionType unregister(std::vector<std::string>*);
ConnectionType myauctions(std::vector<std::string>*);
ConnectionType exituser(std::vector<std::string>*);

const std::unordered_map<std::string, CommandFunction> command_map = {
    {"LIN", login},
    {"LOU", logout},
    {"UNR", unregister},
    {"LMA", myauctions}
};

std::vector<std::string> parse_buffer(char *buffer) {

    // Make the buffer a cpp string
    std::string str(buffer);
    
    // Split the string into tokens
    std::string delimiter = " ";
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter[0])) {
        tokens.push_back(token);
    }

    return tokens;
}

int handle_request(char *buffer) {

    std::vector<std::string> tokens = parse_buffer(buffer);
    auto it = command_map.find(tokens[0]);

    // If the command is found, execute the associated function and return its value
    if (it != command_map.end()) {
        ConnectionType result = it->second(&tokens);
        return result;
    }

    // If the command is not found, return an error value
    return ConnectionType::INVALID;
}

ConnectionType login(std::vector<std::string> token) {
    
    return ConnectionType::UDP;
}

ConnectionType logout(std::vector<std::string> token) {
    
    return ConnectionType::UDP;
}

ConnectionType unregister(std::vector<std::string> token) {
    
    return ConnectionType::UDP;
}

ConnectionType myauctions(std::vector<std::string> token) {
    
    return ConnectionType::UDP;
}

