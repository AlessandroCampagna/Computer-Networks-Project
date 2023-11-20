#ifndef DATA_H
#define DATA_H

#include <filesystem>
#include <vector>
#include <fstream>

#define ASDIR_PATH "../../ASDIR/"
#define USER_PATH "../../ASDIR/USERS/"
#define AUCTION_PATH "../../ASDIR/AUCTIONS/"

// User functions
bool isUser(std::string uid);
int createUser(std::string uid, std::string password);
int removeUser(std::string uid);
bool isPassword(std::string uid, std::string password);
bool isLogin(std::string uid);
int loginUser(std::string uid);
int logoutUser(std::string uid);

// Auction functions
bool isAuction(std::string uid, std::string aid);
bool areAuctions(std::string uid);
std::vector <std::string> getAuctions(std::string uid);

#endif