#include "handle.h"

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

    std::vector<std::string> tokens;
    tokens = parse_buffer(buffer);

    // Handle the request
    if (tokens[0] == "LIN") {
        handle_login(tokens);
    } else {
        printf("Invalid request\n");
    }

    return 0;
}

int handle_login(std::vector<std::string> token) {
    
    return 0;
}

