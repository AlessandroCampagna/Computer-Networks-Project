#include "user.hpp"

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<ConnectionType(Tokens *)>;
using ResponseFunction = std::function<void(Tokens *)>;

ConnectionType handle_command(char *buffer);
void handle_response(char *buffer);

Tokens parse_buffer(char *buffer);
void diparse_buffer(char *buffer, Tokens *tokens);

ConnectionType login(Tokens *);
void login_response(Tokens *);
ConnectionType logout(Tokens *);
void logout_response(Tokens *);
ConnectionType unregister(Tokens *);
void unregister_response(Tokens *);
ConnectionType myauctions(Tokens *);
void myauctions_response(Tokens *);
ConnectionType mybids(Tokens *);
void mybids_response(Tokens *);
ConnectionType list(Tokens *);
void list_response(Tokens *);
ConnectionType show_record(Tokens *);
void show_record_response(Tokens *);

ConnectionType open(Tokens *);
void open_response(Tokens *);
ConnectionType close_auction(Tokens *);
void close_response(Tokens *);
ConnectionType show_asset(Tokens *);
void show_asset_response(Tokens *);
ConnectionType bid(Tokens *);
void bid_response(Tokens *);

ConnectionType exituser(Tokens *);

const std::unordered_map<std::string, CommandFunction> command_map = {
    {"login", login},
    {"logout", logout},
    {"unregister", unregister},
    {"myauctions", myauctions},
    {"ma", myauctions},
    {"mybids", mybids},
    {"mb", mybids},
    {"list", list},
    {"l", list},
    {"show_record", show_record},
    {"sr", show_record},
    {"open", open},
    {"close", close_auction},
    {"show_asset", show_asset},
    {"sa", show_asset},
    {"bid", bid},
    {"b", bid},
    {"exit", exituser}};

const std::unordered_map<std::string, ResponseFunction> response_map = {
    {"RLI", login_response},
    {"RLO", logout_response},
    {"RUR", unregister_response},
    {"RMA", myauctions_response},
    {"RMB", mybids_response},
    {"RLS", list_response},
    {"RRC", show_record_response},
    {"ROA", open_response},
    {"RCL", close_response},
    {"RSA", show_asset_response},
    {"RBD", bid_response}};

std::string uid;
std::string password;
bool logged = false;

ConnectionType handle_command(char *buffer)
{
    Tokens tokens = parse_buffer(buffer);

    auto it = command_map.find(tokens[0]);

    // If the command is found, execute the associated function and return its value
    if (it != command_map.end())
    {
        ConnectionType result = it->second(&tokens);
        diparse_buffer(buffer, &tokens);
        return result;
    }

    // If the command is not found, return an error value
    return ConnectionType::INVALID;
}

void handle_response(char *buffer)
{
    Tokens tokens = parse_buffer(buffer);
    auto it = response_map.find(tokens[0]);

    // If the command is found, execute the associated function and return its value
    if (it != response_map.end())
    {
        it->second(&tokens);
    }

    return;
}

Tokens parse_buffer(char *buffer)
{
    buffer[strlen(buffer) - 1] = '\0';
    // Make the buffer a cpp string
    std::string str(buffer);

    // Split the string into tokens
    std::string delimiter = " ";
    Tokens tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter[0]))
    {
        tokens.push_back(token);
    }

    return tokens;
}

void diparse_buffer(char *buffer, Tokens *tokens)
{
    std::string result = "";
    for (auto word : *tokens)
    {
        result += word + " ";
    }
    result.pop_back(); // remove the last space
    result += "\n";
    std::strcpy(buffer, result.c_str());
}

bool validator(const std::string &uid, const std::string &password)
{
    // Check if uid is up to 6 digits and numeric
    std::regex uidRegex("\\d{1,6}");
    if (!std::regex_match(uid, uidRegex))
    {
        return false;
    }

    // Check if password is 8 digits and alphanumeric
    std::regex passwordRegex("[a-zA-Z0-9]{8}");
    if (!std::regex_match(password, passwordRegex))
    {
        return false;
    }

    return true;
}

// -------------------- UDP -------------------- //

// Define the functions for each command
ConnectionType login(Tokens *tokens)
{
    if ((tokens->size() != 3) || (!validator((*tokens)[1], (*tokens)[2])))
    {
        return ConnectionType::INVALID;
    }
    else
    {
        (*tokens)[0] = "LIN";
        uid = (*tokens)[1];
        password = (*tokens)[2];
        return ConnectionType::UDP;
    }
}

void login_response(Tokens *tokens)
{
    if ((*tokens)[1] == "OK")
    {
        printf("successful login\n");
        logged = true;
    }
    else if ((*tokens)[1] == "NOK")
    {
        printf("incorrect login attempt\n");
    }
    else if ((*tokens)[1] == "REG")
    {
        printf("new user registered\n");
    }
    else
    {
        printf("unknown response\n");
    }
}

ConnectionType logout(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        return ConnectionType::INVALID;
    }
    else
    {
        (*tokens)[0] = "LOU";
        tokens->push_back(uid);
        tokens->push_back(password);
        return ConnectionType::UDP;
    }
}

void logout_response(Tokens *tokens)
{
    if ((*tokens)[1] == "OK")
    {
        printf("successful logout\n");
        logged = false;
    }
    else if ((*tokens)[1] == "NOK")
    {
        printf("user not logged in\n");
    }
    else if ((*tokens)[1] == "UNR")
    {
        printf("unknown user\n");
    }
    else
    {
        printf("unknown response\n");
    }
}

ConnectionType unregister(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        return ConnectionType::INVALID;
    }
    else
    {
        (*tokens)[0] = "UNR";
        tokens->push_back(uid);
        tokens->push_back(password);
        return ConnectionType::UDP;
    }
}

void unregister_response(Tokens *tokens)
{

    if ((*tokens)[1] == "OK")
    {
        logged = false;
        printf("successful unregister\n");
    }
    else if ((*tokens)[1] == "NOK")
    {
        printf("incorrect unregister attempt\n");
    }
    else if ((*tokens)[1] == "UNR")
    {
        printf("unknown user\n");
    }
    else
    {
        printf("unknown response\n");
    }
}

ConnectionType myauctions(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        return ConnectionType::INVALID;
    }
    else
    {
        (*tokens)[0] = "LMA";
        tokens->push_back(uid);
        return ConnectionType::UDP;
    }
}

void myauctions_response(Tokens *tokens)
{

    if ((*tokens)[1] == "OK")
    {
        for (auto it = tokens->begin() + 2; it != tokens->end(); it += 2)
        {
            std::string aid = *it;
            std::string state = *(it + 1);
            printf("Auction ID: %s, State: %s\n", aid.c_str(), state == "1" ? "Active" : "Inactive");
        }
    }
    else if ((*tokens)[1] == "NOK")
    {
        printf("user has no ongoing auctions\n");
    }
    else if ((*tokens)[1] == "NLG")
    {
        printf("user not logged in\n");
    }
    else
    {
        printf("unknown response\n");
    }
}

ConnectionType mybids(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        return ConnectionType::INVALID;
    }
    else
    {
        (*tokens)[0] = "LMB";
        tokens->push_back(uid);
        return ConnectionType::UDP;
    }
}

void mybids_response(Tokens *tokens)
{

    if ((*tokens)[1] == "OK")
    {
        for (auto it = tokens->begin() + 2; it != tokens->end(); it += 2)
        {
            std::string aid = *it;
            std::string state = *(it + 1);
            printf("Auction ID: %s, State: %s\n", aid.c_str(), state == "1" ? "Active" : "Inactive");
        }
    }
    else if ((*tokens)[1] == "NOK")
    {
        printf("user has no ongoing bids\n");
    }
    else if ((*tokens)[1] == "NLG")
    {
        printf("user not logged in\n");
    }
    else
    {
        printf("unknown response\n");
    }
}

ConnectionType list(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        return ConnectionType::INVALID;
    }
    else
    {
        (*tokens)[0] = "LST";
        return ConnectionType::UDP;
    }
}

void list_response(Tokens *tokens)
{
    if ((*tokens)[1] == "OK")
    {
        for (auto it = tokens->begin() + 2; it != tokens->end(); it += 2)
        {
            std::string aid = *it;
            std::string state = *(it + 1);
            printf("Auction ID: %s, State: %s\n", aid.c_str(), state == "1" ? "Active" : "Inactive");
        }
    }
    else if ((*tokens)[1] == "NOK")
    {
        printf("no auctions are currently active\n");
    }
    else
    {
        printf("unknown response\n");
    }
}

ConnectionType show_record(Tokens *tokens)
{
    if (tokens->size() != 2)
    {
        return ConnectionType::INVALID;
    }
    else
    {
        (*tokens)[0] = "SRC";
        return ConnectionType::UDP;
    }
}

void show_record_response(Tokens *tokens)
{
    if ((*tokens)[1] == "NOK")
    {
        printf("Auction does not exist\n");
    }
    else if ((*tokens)[1] == "OK")
    {
        printf("Host UID: %s\n", (*tokens)[2].c_str());
        printf("Auction Name: %s\n", (*tokens)[3].c_str());
        printf("Asset File Name: %s\n", (*tokens)[4].c_str());
        printf("Start Value: %s\n", (*tokens)[5].c_str());
        printf("Start Date and Time: %s\n", (*tokens)[6].c_str());
        printf("Auction Duration: %s seconds\n", (*tokens)[7].c_str());

        int startIndex = 8;
        while ((*tokens)[startIndex] == "B")
        {
            printf("Bidder UID: %s\n", (*tokens)[startIndex + 1].c_str());
            printf("Bid Value: %s\n", (*tokens)[startIndex + 2].c_str());
            printf("Bid Date and Time: %s\n", (*tokens)[startIndex + 3].c_str());
            printf("Bid Time Elapsed: %s seconds\n\n", (*tokens)[startIndex + 4].c_str());
            startIndex += 5;
        }

        if ((*tokens)[tokens->size() - 2] == "E")
        {
            printf("Auction Closing Date and Time: %s\n", (*tokens)[tokens->size() - 2].c_str());
            printf("Auction Closing Time Elapsed: %s seconds\n", (*tokens)[tokens->size() - 1].c_str());
        }
    }
}

// -------------------- TCP -------------------- //

ConnectionType open(Tokens *tokens)
{

    if (tokens->size() != 5)
    {
        return ConnectionType::INVALID;
    }

    std::string fileName = (*tokens)[2];
    tokens->erase(tokens->begin() + 2);

    if (!std::filesystem::exists(ASSETS_PATH + fileName))
        return ConnectionType::INVALID;
    std::uintmax_t fileSize = std::filesystem::file_size(ASSETS_PATH + fileName);

    std::ifstream file(ASSETS_PATH + fileName, std::ios::binary);
    if (!file)
        return ConnectionType::INVALID;
    std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    (*tokens)[0] = "OPA";
    tokens->insert(tokens->begin() + 1, uid);
    tokens->insert(tokens->begin() + 2, password);
    tokens->push_back(fileName);
    tokens->push_back(std::to_string(fileSize));
    tokens->push_back(fileData);

    return ConnectionType::TCP;
}

void open_response(Tokens *tokens)
{

    std::string status = (*tokens)[1];
    std::string AID = (*tokens)[2];

    if (status == "NOK")
    {
        printf("auction could not be started\n");
    }
    else if (status == "NLG")
    {
        printf("user was not logged in\n");
    }
    else if (status == "OK")
    {
        printf("AID: %s\n", AID.c_str());
    }
    else
    {
        printf("unknown status\n");
    }
}

ConnectionType close_auction(Tokens *tokens)
{
    if (tokens->size() != 2)
    {
        return ConnectionType::INVALID;
    }

    (*tokens)[0] = "CLS";
    tokens->insert(tokens->begin() + 1, uid);
    tokens->insert(tokens->begin() + 2, password);

    return ConnectionType::TCP;
}

void close_response(Tokens *tokens)
{
    std::string status = (*tokens)[1];

    if (status == "OK")
    {
        printf("Auction was successfully closed.\n");
    }
    else if (status == "NLG")
    {
        printf("User was not logged in.\n");
    }
    else if (status == "EAU")
    {
        printf("Auction does not exist.\n");
    }
    else if (status == "EOW")
    {
        printf("Auction is not owned by user.\n");
    }
    else if (status == "END")
    {
        printf("Auction owned by user has already finished.\n");
    }
    else
    {
        printf("Unknown status.\n");
    }
}

ConnectionType show_asset(Tokens *tokens)
{
    if (tokens->size() != 2)
    {
        return ConnectionType::INVALID;
    }

    (*tokens)[0] = "SAS";

    return ConnectionType::TCP;
}

void show_asset_response(Tokens *tokens)
{
    std::string status = (*tokens)[1];

    if (status == "OK")
    {
        std::string fileName = (*tokens)[2];
        std::string fileSize = (*tokens)[3];
        std::string fileData = (*tokens)[4];

        std::ofstream file(ASSETS_PATH + fileName);
        file << fileData;
        file.close();

        printf("Asset in directory: %s\n", ASSETS_PATH);
    }
    else if (status == "NOK")
    {
        printf("Asset does not exist.\n");
    }
    else
    {
        printf("Unknown status.\n");
    }
}

ConnectionType bid(Tokens *tokens)
{
    if (tokens->size() != 3)
    {
        return ConnectionType::INVALID;
    }

    (*tokens)[0] = "BID";
    tokens->insert(tokens->begin() + 1, uid);
    tokens->insert(tokens->begin() + 2, password);

    return ConnectionType::TCP;
}

void bid_response(Tokens *tokens)
{
    std::string status = (*tokens)[1];

    if (status == "NLG")
    {
        printf("User was not logged in.\n");
    }
    else if (status == "NOK")
    {
        printf("Auction is not active.\n");
    }
    else if (status == "ACC")
    {
        printf("Bid was accepted.\n");
    }
    else if (status == "REF")
    {
        printf("Bid was refused because a larger bid has already been placed.\n");
    }
    else if (status == "ILG")
    {
        printf("User cannot make a bid in an auction hosted by themselves.\n");
    }
    else
    {
        printf("Unknown status.\n");
    }
}

// -------------------- EXIT -------------------- //

ConnectionType exituser(Tokens *tokens)
{
    return ConnectionType::EXIT;
}