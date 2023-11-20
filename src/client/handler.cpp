#include "user.h"

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<ConnectionType(Tokens*)>;
using ResponseFunction = std::function<void(Tokens*)>;

Tokens parse_buffer(char *buffer);
void diparse_buffer(char* buffer, Tokens* tokens);

ConnectionType login(Tokens*); void login_response(Tokens*);
ConnectionType logout(Tokens*); void logout_response(Tokens*);
ConnectionType unregister(Tokens*); void unregister_response(Tokens*);
ConnectionType myauctions(Tokens*); void myauctions_response(Tokens*);
ConnectionType mybids(Tokens*); void mybids_response(Tokens*);
ConnectionType list(Tokens*); void list_response(Tokens*);
ConnectionType show_record(Tokens*); void show_record_response(Tokens*);
ConnectionType exituser(Tokens*);

const std::unordered_map<std::string, CommandFunction> command_map = {
    {"login", login},
    {"logout", logout},
    {"unregister", unregister},
    {"myauctions", myauctions},
    {"ma", myauctions},
    {"mybids", mybids},
    {"mb", mybids},
    {"list", list},
    {"l", list},
    {"show_record", show_record},
    {"sr", show_record},
    {"exit", exituser}
};

const std::unordered_map<std::string, ResponseFunction> response_map = {
    {"RLI", login_response},
    {"RLO", logout_response},
    {"RUR", unregister_response},
    {"RMA", myauctions_response},
    {"RMB", mybids_response},
    {"RLS", list_response},
    {"RRC", show_record_response}
};

std::string uid;
std::string password;
bool logged=false;

ConnectionType handle_command(char* buffer) {

    buffer[strlen(buffer)-1] = '\0'; // remove the last \n
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

void handle_response(char* buffer){
    
    buffer[strlen(buffer)-1] = '\0'; // remove the last \n
    Tokens tokens = parse_buffer(buffer);
    auto it = response_map.find(tokens[0]);

    // If the command is found, execute the associated function and return its value
    if (it != response_map.end()) {
        it->second(&tokens);
    }

    return;
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
    result += "\n";
    std::strcpy(buffer, result.c_str());
}

// Define the functions for each command
ConnectionType login(Tokens* tokens) {
    (*tokens)[0] = "LIN";
    uid = (*tokens)[1];
    password = (*tokens)[2];
    return ConnectionType::UDP;
}

void login_response(Tokens* tokens) {
    if ((*tokens)[1] == "OK") {
        printf("successful login\n");
        logged = true;
    } else if ((*tokens)[1] == "NOK") {
        printf("incorrect login attempt\n");
    } else if ((*tokens)[1] == "REG") {
        printf("new user registered\n");
    } else {
        printf("unknown response\n");
    }
}

ConnectionType logout(Tokens* tokens) {
    (*tokens)[0] = "LOU";
    tokens->push_back(uid);
    tokens->push_back(password);
    return ConnectionType::UDP;
}

void logout_response(Tokens* tokens) {
    if ((*tokens)[1] == "OK") {
        printf("successful logout\n");
        logged = false;
    } else if ((*tokens)[1] == "NOK") {
        printf("user not logged in\n");
    } else if ((*tokens)[1] == "UNR") {
        printf("unknown user\n");
    } else {
        printf("unknown response\n");
    }
}

ConnectionType unregister(Tokens* tokens) {
    (*tokens)[0] = "UNR";
    tokens->push_back(uid);
    tokens->push_back(password);
    return ConnectionType::UDP;
}

void unregister_response(Tokens* tokens) {
    
    if ((*tokens)[1] == "OK") {
        printf("successful unregister\n");
    } else if ((*tokens)[1] == "NOK") {
        printf("incorrect unregister attempt\n");
    } else if ((*tokens)[1] == "UNR") {
        printf("unknown user\n");
    } else {
        printf("unknown response\n");
    }
}


ConnectionType myauctions(Tokens* tokens) {
    (*tokens)[0] = "LMA";
    tokens->push_back(uid);
    return ConnectionType::UDP;
}

void myauctions_response(Tokens* tokens) {
    
    if ((*tokens)[1] == "OK") {
        for (auto it = tokens->begin() + 2; it != tokens->end(); ++it) {
            printf("%s\n", it->c_str());
        }
    } else if ((*tokens)[1] == "NOK") {
        printf("user has no ongoing auctions\n");
    } else if ((*tokens)[1] == "NLG") {
        printf("user not logged in\n");
    } else {
        printf("unknown response\n");
    }
}

ConnectionType mybids(Tokens* tokens){
    (*tokens)[0] = "LMB";
    tokens->push_back(uid);
    return ConnectionType::UDP;
}

void mybids_response(Tokens* tokens){
    
    if ((*tokens)[1] == "OK") {
        for (auto it = tokens->begin() + 2; it != tokens->end(); ++it) {
            printf("%s\n", it->c_str());
        }
    } else if ((*tokens)[1] == "NOK") {
        printf("user has no ongoing bids\n");
    } else if ((*tokens)[1] == "NLG") {
        printf("user not logged in\n");
    } else {
        printf("unknown response\n");
    }
}

ConnectionType list(Tokens* tokens){
    (*tokens)[0] = "LST";
    return ConnectionType::UDP;
}

void list_response(Tokens* tokens){
     if ((*tokens)[1] == "OK") {
        for (auto it = tokens->begin() + 2; it != tokens->end(); ++it) {
            printf("%s\n", it->c_str());
        }
    } else if ((*tokens)[1] == "NOK") {
        printf("no auctions are currently active\n");
    } else {
        printf("unknown response\n");
    }
}

ConnectionType show_record(Tokens* tokens){
    (*tokens)[0] = "SRC";
    return ConnectionType::UDP;
}

void  show_record_response(Tokens* token){

}

ConnectionType exituser(Tokens* tokens) {
    return ConnectionType::EXIT;
}


