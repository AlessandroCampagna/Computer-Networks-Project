#include "data.h"

namespace fs = std::filesystem;

// User functions
bool isUser(std::string uid)
{
    // Check if user directory exists
    if (fs::exists(USER_PATH + uid))
        return true;
    return false;
}

int createUser(std::string uid, std::string password)
{
    printf("(AS) Creating user %s\n", uid.c_str());
    // Create user directory
    fs::create_directories(USER_PATH + uid);

    // Create password file
    std::ofstream file(USER_PATH + uid + "/" + uid + "_pass.txt");
    file << password;
    file.close();

    // Create hosted directory
    fs::create_directories(USER_PATH + uid + "/HOSTED");
    // Create bid directory
    fs::create_directories(USER_PATH + uid + "/BID");

    return 0;
}

int removeUser(std::string uid)
{
    printf("(AS) Removing user %s\n", uid.c_str());
    // Remove user directory
    fs::remove_all(USER_PATH + uid + "/" + uid + "_pass.txt");
    fs::remove_all(USER_PATH + uid + "/" + uid + "_login.txt");

    return 0;
}

bool isPassword(std::string uid, std::string password)
{
    // Check if the password is correct
    std::ifstream file(USER_PATH + uid + "/" + uid + "_pass.txt");
    std::string correct_password;
    std::getline(file, correct_password);
    if (password == correct_password)
        return true;
    return false;
}

bool isLogin(std::string uid)
{
    // Check if user directory exists
    if (fs::exists(USER_PATH + uid + "/" + uid + "_login.txt"))
        return true;
    return false;
}

int loginUser(std::string uid)
{
    // Create login file
    printf("(AS) Creating login file\n");
    std::ofstream file(USER_PATH + uid + "/" + uid + "_login.txt");
    return 0;
}

int logoutUser(std::string uid)
{
    // Remove login file
    printf("(AS) Removing login file\n");
    fs::remove(USER_PATH + uid + "/" + uid + "_login.txt");
    return 0;
}

// Auction fucntions

bool isAuction(std::string uid, std::string aid)
{
    // Check if auction directory exists
    if (fs::exists(USER_PATH + uid + "/HOSTED/" + aid + ".txt"))
        return true;
    return false;
}

bool areAuctions(std::string uid)
{
    // Check if inside auction directory exists any file
    for (const auto &entry : fs::directory_iterator(USER_PATH + uid + "/HOSTED/"))
    {
        (void)entry; // Cast to void to avoid unused variable warning
        return true;
    }
    return false;
}

std::vector<std::string> getAuctions(std::string uid)
{
    std::vector<std::string> auctions;
    for (const auto &entry : fs::directory_iterator(USER_PATH + uid + "/HOSTED/"))
    {
        std::string auction = entry.path();
        auction = auction.substr(auction.find_last_of("/") + 1);
        auction = auction.substr(0, auction.find_last_of("."));
        auctions.push_back(auction);
    }
    return auctions;
}