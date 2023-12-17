#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <stdio.h>
#include <cstdio>
#include <unordered_map>
#include <functional>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <regex>
#include <filesystem>
#include <fstream>
#include <csignal>

#include "connection.hpp"

#define ASSETS_PATH "../../ASSETS/"

using Tokens = std::vector<std::string>;
using CommandFunction = std::function<void(Tokens *)>;

void handle_buffer();
Tokens parse_buffer();
void diparse_buffer(Tokens *);
void signalHandler(int);

void login(Tokens *);
void login_response(Tokens *);
void logout(Tokens *);
void logout_response(Tokens *);
void unregister(Tokens *);
void unregister_response(Tokens *);
void myauctions(Tokens *);
void myauctions_response(Tokens *);
void mybids(Tokens *);
void mybids_response(Tokens *);
void list(Tokens *);
void list_response(Tokens *);
void clear(Tokens *);
void show_record(Tokens *);
void show_record_response(Tokens *);

void open(Tokens *);
void open_response(Tokens *);
void close_auction(Tokens *);
void close_response(Tokens *);
void show_asset(Tokens *);
void show_asset_response(Tokens *);
void bid(Tokens *);
void bid_response(Tokens *);

void exituser(Tokens *);

#endif