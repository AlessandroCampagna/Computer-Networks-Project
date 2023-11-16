// UDP
#include "user.h"

// Define the function pointers for the commands
using CommandFunction = std::function<ConnectionType(char*)>;

// Define the hash map to store the commands and their associated functions
std::unordered_map<std::string, CommandFunction> command_map;

char uid[7];
char password[9];

// Define the functions for each command
ConnectionType login(std::vector<std::string>* buffer_str) {
    buffer_str[0] = 'LIN';
    uid = buffer_str[1];
    password = buffer_str[2];
    return ConnectionType::UDP;
}

ConnectionType logout(std::vector<std::string>* buffer_str) {
    buffer_str[0] = 'LOU';
    buffer_str->push_back(uid);
    buffer_str->push_back(password);
    return ConnectionType::UDP;
}

ConnectionType unregister(std::vector<std::string>* buffer_str) {
    buffer_str[0] = 'UNR';
    buffer_str->push_back(uid);
    buffer_str->push_back(password);
    return ConnectionType::UDP;
}

ConnectionType myauctions(std::vector<std::string>* buffer_str) {
    buffer_str[0] = 'LMA';
    buffer_str->push_back(uid);
    return ConnectionType::UDP;
}

ConnectionType exit(std::vector<std::string>* buffer_str) {
    return ConnectionType::EXIT;
}

// Add the commands and their associated functions to the hash map
command_map["login"] = login;
command_map["logout"] = logout;
command_map["unregister"] = unregister;
command_map["myauctions"] = myauctions;
command_map["exit"] = exit;

// Define the user_command function
ConnectionType user_command(char* buffer) {
    
    std::vector<std::string> buffer_words = split_string(buffer);
    auto it = command_map.find(buffer_words[0]);

    // If the command is found, execute the associated function and return its value
    if (it != command_map.end()) {
        ConnectionType result = it->second(buffer_words);
        unsplit_string(buffer, &buffer_words);
        return result;
    }

    // If the command is not found, return an error value
    return ConnectionType::INVALID;
}

std::vector<std::string> split_string(char* buffer) {
    std::string str(buffer);
    std::vector<std::string> words;
    std::string word = "";
    for (auto c : str) {
        if (c == ' ') {
            words.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    words.push_back(word);
    return words;
}

void unsplit_string(char* buffer, std::vector<std::string>* words) {
    std::string result = "";
    for (auto word : words) {
        result += word + " ";
    }
    std::strcpy(buffer, result.c_str());
}


