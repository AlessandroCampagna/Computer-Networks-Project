#include "user.h"

using CommandFunction = std::function<ConnectionType(std::vector<std::string>*)>;

std::vector<std::string> split_string(char*);
void unsplit_string(char*, std::vector<std::string>*);

ConnectionType login(std::vector<std::string>*);
ConnectionType logout(std::vector<std::string>*);
ConnectionType unregister(std::vector<std::string>*);
ConnectionType myauctions(std::vector<std::string>*);
ConnectionType exituser(std::vector<std::string>*);

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
    
    std::vector<std::string> buffer_words = split_string(buffer);
    auto it = command_map.find(buffer_words[0]);

    // If the command is found, execute the associated function and return its value
    if (it != command_map.end()) {
        ConnectionType result = it->second(&buffer_words);
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
        if ((c == ' ') or (c == '\n')) {
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
    for (auto word : *words) {
        result += word + " ";
    }
    std::strcpy(buffer, result.c_str());
}

// Define the functions for each command
ConnectionType login(std::vector<std::string>* buffer_str) {
    (*buffer_str)[0] = "LIN";
    uid = (*buffer_str)[1];
    password = (*buffer_str)[2];
    return ConnectionType::UDP;
}

ConnectionType logout(std::vector<std::string>* buffer_str) {
    (*buffer_str)[0] = "LOU";
    buffer_str->push_back(uid);
    buffer_str->push_back(password);
    return ConnectionType::UDP;
}

ConnectionType unregister(std::vector<std::string>* buffer_str) {
    (*buffer_str)[0] = "UNR";
    buffer_str->push_back(uid);
    buffer_str->push_back(password);
    return ConnectionType::UDP;
}

ConnectionType myauctions(std::vector<std::string>* buffer_str) {
    (*buffer_str)[0] = "LMA";
    buffer_str->push_back(uid);
    return ConnectionType::UDP;
}

ConnectionType exituser(std::vector<std::string>* buffer_str) {
    return ConnectionType::EXIT;
}


