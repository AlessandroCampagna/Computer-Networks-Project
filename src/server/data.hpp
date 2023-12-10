#ifndef DATA_H
#define DATA_H

#include <filesystem>
#include <vector>
#include <fstream>

#define ASDIR_PATH "../../ASDIR/"
#define USER_PATH "../../ASDIR/USERS/"
#define AUCTION_PATH "../../ASDIR/AUCTIONS/"
#define HOSTED "/HOSTED/"
#define BIDDED "/BIDDED/"
#define ASSET "/ASSET/"
#define BIDS "/BIDS/"
#define END "/END_"
#define START "START_"

// User functions
bool isUser(std::string uid);
int createUser(std::string uid, std::string password);
int removeUser(std::string uid);
bool isPassword(std::string uid, std::string password);
bool isLogin(std::string uid);
int loginUser(std::string uid);
int logoutUser(std::string uid);
bool autenticateUser(std::string uid, std::string password);

// Auction functions
bool isAuction(std::string uid, std::string aid);
bool areUserAuctions(std::string uid);
std::vector<std::string> getAuctions(std::string uid);
std::string createAuction(std::string uid, std::string name,
                  std::string startValue, std::string timeActive,
                  std::string fileName, std::string fileSize,
                  std::string fileData);
int removeAuction(std::string aid);
int createBid(std::string uid, std::string aid, std::string value);

#endif