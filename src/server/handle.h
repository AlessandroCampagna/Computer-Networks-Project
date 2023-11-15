#ifndef HANDLE_H
#define HANDLE_H

#include <vector>
#include <sstream>
#include <string>
#include <cstdio>

std::vector<std::string> parse_buffer(char *buffer);
int handle_request(char *buffer);

#endif