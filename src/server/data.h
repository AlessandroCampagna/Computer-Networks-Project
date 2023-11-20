#ifndef DATA_H
#define DATA_H

#include <filesystem>
#include <vector>
#include <fstream>

#define ASDIR_PATH "../../ASDIR/"
#define USER_PATH "../../ASDIR/USERS/"
#define AUCTION_PATH "../../ASDIR/AUCTIONS/"

int isUser(std::string uid);
int createUser(std::string uid, std::string password);
int removeUser(std::string uid);
int isPassword(std::string uid, std::string password);
int isLogin(std::string uid);
int loginUser(std::string uid);
int logoutUser(std::string uid);

#endif