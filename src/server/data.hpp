#ifndef DATA_H
#define DATA_H

#include <filesystem>
#include <vector>
#include <fstream>

#define ASDIR_PATH "../../ASDIR/"
#define TEMP_PATH "../../ASDIR/temp"
#define USER_PATH "../../ASDIR/USERS/"
#define AUCTION_PATH "../../ASDIR/AUCTIONS/"
#define HOSTED "/HOSTED/"
#define BIDDED "/BIDDED/"
#define ASSET "/ASSET/"
#define BIDS "/BIDS/"
#define END "/END_"
#define START "/START_"

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
std::vector<std::string> getAllAuctions();
std::string createAuction(std::string uid, std::string name,
                          std::string startValue, std::string timeActive,
                          std::string fileName, std::string fileSize);
int removeAuction(std::string aid);
bool isAuctionOpen(std::string aid);
bool isAuction(std::string aid);
std::string getAuctionHostUid(std::string aid);
std::string getAuctionName(std::string aid);
std::string getAuctionAssetFilename(std::string aid);
std::string getAuctionStartValue(std::string aid);
std::string getAuctionTimeActive(std::string aid);

int createBid(std::string uid, std::string aid, std::string value);
std::vector<std::string> getAuctionsBidded(std::string uid);

int loadAsset(std::string aid);
std::string getAssetName(std::string aid);
std::string getAssetSize(std::string aid);

#endif