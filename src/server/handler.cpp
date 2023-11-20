#include "server.h"
#include "data.h"

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<ConnectionType(Tokens*)>;

ConnectionType login(Tokens*);
ConnectionType logout(Tokens*);
ConnectionType unregister(Tokens*);
ConnectionType myauctions(Tokens*);
ConnectionType exituser(Tokens*);

const std::unordered_map<std::string, CommandFunction> command_map = {
    {"LIN", login},
    {"LOU", logout},
    {"UNR", unregister},
    {"LMA", myauctions}
};

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

ConnectionType handle_request(char *buffer) {

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

ConnectionType login(Tokens* token) {
    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];

    //Check if the user exists in the database
    if (isUser(uid) == 0) {
        createUser(uid, password);
        loginUser(uid);
        response.push_back("RLI");
        response.push_back("REG");
    // Not loged in
    } else {
        //Check if the password is correct
        if (isPassword(uid, password) == 0) {
            response.push_back("RLI");
            response.push_back("NOK");
        } else {
            loginUser(uid);
            response.push_back("RLI");
            response.push_back("OK");
        }
    }

    // Replace token with response
    *token = response;
    
    return ConnectionType::UDP;
}

ConnectionType logout(Tokens* token) {
    // Create new token for response
    Tokens response;

    std::string uid = (*token)[1];
    std::string password = (*token)[2];
    
    //Check if the user dose not exists in the database
    if (isUser(uid) == 1) {
        response.push_back("LOU");
        response.push_back("UNR"); 
    } else if (isLogin(uid) == 0) {
        response.push_back("LOU");
        response.push_back("NOK"); 
    } else {
        logoutUser(uid);
        response.push_back("LOU");
        response.push_back("OK");
    }

    *token = response;

    return ConnectionType::UDP;
}

ConnectionType unregister(Tokens* token) {
    
    return ConnectionType::UDP;
}

ConnectionType myauctions(Tokens* token) {
    
    return ConnectionType::UDP;
}