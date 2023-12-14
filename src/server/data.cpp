#include "data.hpp"

//TODO: Implement fs erros logic

namespace fs = std::filesystem;

// User functions

// Check if the user exists in the database
bool isUser(std::string uid)
{
    if (fs::exists(USER_PATH + uid))
        return true;
    return false;
}

// Create a user in the database
int createUser(std::string uid, std::string password)
{
    printf("(AS) Creating user %s\n", uid.c_str());

    fs::create_directories(USER_PATH + uid);

    std::ofstream file(USER_PATH + uid + "/" + uid + "_pass.txt");
    file << password;
    file.close();

    fs::create_directories(USER_PATH + uid + "/HOSTED");
    fs::create_directories(USER_PATH + uid + "/BID");

    return 0;
}

// Remove a user from the database
int removeUser(std::string uid)
{
    printf("(AS) Removing user %s\n", uid.c_str());
    fs::remove_all(USER_PATH + uid + "/" + uid + "_pass.txt");
    fs::remove_all(USER_PATH + uid + "/" + uid + "_login.txt");

    return 0;
}

// Check if the password is correct for the user in the database
bool isPassword(std::string uid, std::string password)
{
    std::ifstream file(USER_PATH + uid + "/" + uid + "_pass.txt");
    std::string correct_password;
    std::getline(file, correct_password);
    if (password == correct_password)
        return true;
    return false;
}

// Check if the user has a login file in the database
bool isLogin(std::string uid)
{
    if (fs::exists(USER_PATH + uid + "/" + uid + "_login.txt"))
        return true;
    return false;
}

// Create a login file for a user in the database
int loginUser(std::string uid)
{
    // Create login file
    printf("(AS) Creating login file\n");
    std::ofstream file(USER_PATH + uid + "/" + uid + "_login.txt");
    return 0;
}

// Remove the login file for a use
int logoutUser(std::string uid)
{
    printf("(AS) Removing login file\n");
    fs::remove(USER_PATH + uid + "/" + uid + "_login.txt");
    return 0;
}

// Validate that user has login file and has correct password in the database
bool autenticateUser(std::string uid, std::string password)
{
    if (isUser(uid) == false)
        return false;
    if (isPassword(uid, password) == false)
        return false;
    return true;
}

// Auction fucntions

// Check if an auction exists for the user in the database
bool isUserAuction(std::string uid, std::string aid)
{
    if (fs::exists(USER_PATH + uid + HOSTED + aid + ".txt"))
        return true;
    return false;
}

// Check if a user has auctions in the database
bool areUserAuctions(std::string uid)
{
    // Check if inside auction directory exists any file
    for (const auto &entry : fs::directory_iterator(USER_PATH + uid + HOSTED))
    {
        (void)entry; // Cast to void to avoid unused variable warning
        return true;
    }
    return false;
}

// Return user auctions in the database
std::vector<std::string> getAuctions(std::string uid)
{
    std::vector<std::string> auctions;
    for (const auto &entry : fs::directory_iterator(USER_PATH + uid + HOSTED))
    {
        std::string auction = entry.path();
        auction = auction.substr(auction.find_last_of("/") + 1);
        auction = auction.substr(0, auction.find_last_of("."));
        auctions.push_back(auction);
    }
    return auctions;
}

std::string findNextAuctionID()
{
    // Check if the directory exists
    if (!fs::exists(AUCTION_PATH)) {
        return "000";
    }

    // Count number of existing auctions
    int count = 0;
    for (const auto &entry : fs::directory_iterator(AUCTION_PATH))
    {
        (void)entry; // Cast to void to avoid unused variable warning
        count++;
    }

    // Convert count to a 3-digit string with leading zeros
    return std::to_string(count / 100 % 10) + std::to_string(count / 10 % 10) + std::to_string(count % 10);
}

std::ifstream openTempFile(const std::string& filename) {
    std::ifstream tempFile(filename, std::ios::binary);
    if (!tempFile) {
        perror("(Handler) Error opening temporary file");
        exit(EXIT_FAILURE);
    }
    return tempFile;
}

// Create an auction for the user in the database
std::string createAuction(std::string uid, std::string name,
                                           std::string startValue, std::string timeActive,
                                           std::string fileName, std::string fileSize)
{
    std::string aid = findNextAuctionID();
    //TODO: Implement datetime logic
    std::string dateTime = "dateTime";
    std::string fullTime = "fullTime";

    // Create auctions directory in auctions directory
    fs::create_directories(AUCTION_PATH + aid);
    // Create asset file
    std::ofstream asset(AUCTION_PATH + aid + ASSET + fileName);
    // Load from the temp file
    std::ifstream tempFile = openTempFile(TEMP_PATH);
    asset << tempFile.rdbuf();
    tempFile.close();

    // Create start file
    std::ofstream start(AUCTION_PATH + aid + START + aid + ".txt");
    start << uid + " " + name + " " + fileName + " " + startValue + " " + 
    timeActive + " " + dateTime + " " + fullTime;

    // Create bids directory
    fs::create_directories(AUCTION_PATH + aid + BIDS);

    // Create auction in user directory
    std::ofstream file(USER_PATH + uid + HOSTED + aid + ".txt");

    return aid;
}

// Close an auction in the database
int removeAuction(std::string aid)
{
    //TODO: Implement datetime logic and error codes
    std::string dateTime = "datetime";
    std::string secTime = "sectime";

    std::ofstream file(AUCTION_PATH + aid + END + aid + ".txt");
    file << dateTime + " " + secTime;

    return 0;
}

// Create a bid in the database
int createBid(std::string uid, std::string aid, std::string value)
{
    // Create bid file on user folder
    std::ofstream userBid(USER_PATH + uid + BIDDED + aid + ".txt");
    
    // Create bid file on auctions
    std::ofstream auctionBid(AUCTION_PATH + aid + BIDS + value + ".txt");
    
    return 0;
}