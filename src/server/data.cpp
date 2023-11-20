#include "data.h"

namespace fs = std::filesystem;

// User functions
int isUser(std::string uid) {
    // Check if user directory exists
    if (fs::exists(USER_PATH + uid)) return 1;
    return 0;
}

int createUser(std::string uid, std::string password) {
    // Create user directory
    fs::create_directories(USER_PATH + uid);

    // Create password file
    std::ofstream file(USER_PATH + uid + "/" + uid + "_pass.txt");
    file << password;
    file.close();

    return 0;
}

int removeUser(std::string uid) {
    // Remove user directory
    fs::remove_all(USER_PATH + uid + "/" + uid + "_pass.txt");
    fs::remove_all(USER_PATH + uid + "/" + uid + "_login.txt");

    return 0;
}

int isPassword(std::string uid, std::string password) {
    //Check if the password is correct
    std::ifstream file(USER_PATH + uid + "/" + password + "_pass.txt");
    std::string correct_password;
    std::getline(file, correct_password);
    if (password != correct_password) return 0;
    return 1;
}

int isLogin(std::string uid) {
    // Check if user directory exists
    if (fs::exists(USER_PATH + uid + "/" + uid + "_login.txt")) return 1;
    return 0;
}

int logoutUser(std::string uid) {
    // Check if user directory exists
    if (fs::exists(USER_PATH + uid)) return -1;
    
    // Remove login file
    fs::remove(USER_PATH + uid + "/" + uid + "_login.txt");

    return 0;
}

// Auction fucntions