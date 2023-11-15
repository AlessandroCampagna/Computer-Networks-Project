#include "process.h"

#include <vector>
#include <sstream>
#include <string>
#include <cstdio>

int process_request(char *buffer) {

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

    printf("tokens[0] = %s\n", tokens[0].c_str());

    return 0;
}