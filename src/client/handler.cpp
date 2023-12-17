#include "handler.hpp"

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
    {"clear", clear},
    {"show_record", show_record},
    {"sr", show_record},
    {"open", open},
    {"close", close_auction},
    {"show_asset", show_asset},
    {"sa", show_asset},
    {"bid", bid},
    {"b", bid},
    {"exit", exituser},
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

void handle_buffer()
{
    Tokens tokens = parse_buffer();
    auto it = command_map.find(tokens[0]);
    if (it != command_map.end())
    {
        if ((logged) || (tokens[0] == "login") || (tokens[0] == "RLI") || (tokens[0] == "exit"))
            it->second(&tokens);
        else
        {
            printf("User not logged in\n");
        }
    }
    else
    {
        printf("Invalid Command\n");
    }
}

Tokens parse_buffer()
{
    if (strlen(buffer) > 1)
        buffer[strlen(buffer) - 1] = '\0';

    std::string str(buffer);
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

void diparse_buffer(Tokens *tokens)
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

void signalHandler(int signum)
{
    if (signum == SIGPIPE)
    {
        // Handle broken pipe signal
        printf("Received SIGPIPE, ignoring it\n");
    }
    else if ((signum == SIGINT) or (signum == SIGTSTP))
    {
        // Handle interrupt signal (CTRL+C)

        printf("\nExiting..\n");
        strcpy(buffer, "exit\n");
        handle_buffer();
    }
}

// -------------------- UDP -------------------- //

void login(Tokens *tokens)
{
    if ((tokens->size() != 3) || (!validator((*tokens)[1], (*tokens)[2])))
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "LIN";
        uid = std::string(6 - (*tokens)[1].length(), '0') + (*tokens)[1];
        (*tokens)[1] = uid;
        password = (*tokens)[2];

        diparse_buffer(tokens);
        send_udp();
        handle_buffer();
    }
}

void login_response(Tokens *tokens)
{
    if ((*tokens)[1] == "OK")
    {
        logged = true;
        printf("successful login\n");
    }
    else if ((*tokens)[1] == "NOK")
    {
        printf("incorrect login attempt\n");
    }
    else if ((*tokens)[1] == "REG")
    {
        logged = true;
        printf("new user registered\n");
    }
    else
    {
        printf("unknown response\n");
    }
}

void logout(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "LOU";
        tokens->push_back(uid);
        tokens->push_back(password);

        diparse_buffer(tokens);
        send_udp();
        handle_buffer();
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

void unregister(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "UNR";
        tokens->push_back(uid);
        tokens->push_back(password);

        diparse_buffer(tokens);
        send_udp();
        handle_buffer();
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

void myauctions(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "LMA";
        tokens->push_back(uid);

        diparse_buffer(tokens);
        send_udp();
        handle_buffer();
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

void mybids(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "LMB";
        tokens->push_back(uid);

        diparse_buffer(tokens);
        send_udp();
        handle_buffer();
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

void list(Tokens *tokens)
{
    if (tokens->size() != 1)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "LST";

        diparse_buffer(tokens);
        send_udp();
        handle_buffer();
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

void show_record(Tokens *tokens)
{
    if (tokens->size() != 2)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "SRC";

        diparse_buffer(tokens);
        send_udp();
        handle_buffer();
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
        printf("Start Date and Time: %s %s\n", (*tokens)[6].c_str(), (*tokens)[7].c_str());
        printf("Auction Duration: %s seconds\n\n", (*tokens)[8].c_str());

        int startIndex = 9;
        while ((*tokens)[startIndex] == "B")
        {
            printf("Bidder UID: %s\n", (*tokens)[startIndex + 1].c_str());
            printf("Bid Value: %s\n", (*tokens)[startIndex + 2].c_str());
            printf("Bid Date and Time: %s %s\n", (*tokens)[startIndex + 3].c_str(), (*tokens)[startIndex + 4].c_str());
            printf("Bid Time Elapsed: %s seconds\n\n", (*tokens)[startIndex + 5].c_str());
            startIndex += 6;
        }

        if ((*tokens)[tokens->size() - 4] == "E")
        {
            printf("Auction Closing Date and Time: %s %s\n", (*tokens)[tokens->size() - 3].c_str(), (*tokens)[tokens->size() - 2].c_str());
            printf("Auction Closing Time Elapsed: %s seconds\n", (*tokens)[tokens->size() - 1].c_str());
        }
    }
}

// -------------------- TCP -------------------- //

void open(Tokens *tokens)
{

    if (tokens->size() != 5)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {

        std::string fileName = (*tokens)[2];
        tokens->erase(tokens->begin() + 2);

        if (!std::filesystem::exists(ASSETS_PATH + fileName))
        {
            printf("Invalid Command\n");
            return;
        }

        std::uintmax_t fileSize = std::filesystem::file_size(ASSETS_PATH + fileName);

        (*tokens)[0] = "OPA";
        tokens->insert(tokens->begin() + 1, uid);
        tokens->insert(tokens->begin() + 2, password);
        tokens->push_back(fileName);
        tokens->push_back(std::to_string(fileSize));

        diparse_buffer(tokens);
        send_tcp(ASSETS_PATH + fileName);
        handle_buffer();
    }
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

void close_auction(Tokens *tokens)
{
    if (tokens->size() != 2)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {
        (*tokens)[0] = "CLS";
        tokens->insert(tokens->begin() + 1, uid);
        tokens->insert(tokens->begin() + 2, password);

        diparse_buffer(tokens);
        send_tcp();
        handle_buffer();
    }
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

void show_asset(Tokens *tokens)
{
    if (tokens->size() != 2)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {

        (*tokens)[0] = "SAS";

        diparse_buffer(tokens);
        send_tcp();
        handle_buffer();
    }
}

void show_asset_response(Tokens *tokens)
{
    std::string status = (*tokens)[1];

    if (status == "OK")
    {
        std::string fileName = (*tokens)[2];
        std::string fileSize = (*tokens)[3];

        std::rename(TEMP_PATH, (ASSETS_PATH + fileName).c_str());

        printf("Asset %s in directory ASSETS\n", fileName.c_str());
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

void bid(Tokens *tokens)
{
    if (tokens->size() != 3)
    {
        printf("Invalid Command\n");
        return;
    }
    else
    {

        (*tokens)[0] = "BID";
        tokens->insert(tokens->begin() + 1, uid);
        tokens->insert(tokens->begin() + 2, password);

        diparse_buffer(tokens);
        send_tcp();
        handle_buffer();
    }
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

void exituser(Tokens *tokens)
{
    if (logged){
        strcpy(buffer, "logout\n");
        handle_buffer();
    }
    close_socket();
    exit(0);
}

void clear(Tokens *tokens)
{
    system("clear");
}