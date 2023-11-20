#include "data.h"

namespace fs = std::filesystem;

// User functions
bool isUser(std::string uid) {
    // Check if user directory exists
    if (fs::exists(USER_PATH + uid)) return true;
    return false;
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

bool isPassword(std::string uid, std::string password) {
    //Check if the password is correct
    std::ifstream file(USER_PATH + uid + "/" + password + "_pass.txt");
    std::string correct_password;
    std::getline(file, correct_password);
    if (password != correct_password) return false;
    return true;
}

bool isLogin(std::string uid) {
    // Check if user directory exists
    if (fs::exists(USER_PATH + uid + "/" + uid + "_login.txt")) return true;
    return false;
}

int loginUser(std::string uid) {
    // Create login file
    printf("Creating login file\n");
    std::ofstream file(USER_PATH + uid + "/" + uid + "_login.txt");
    return 0;
}

int logoutUser(std::string uid) {
    // Remove login file
    fs::remove(USER_PATH + uid + "/" + uid + "_login.txt");
    return 0;
}

// Auction fucntions