#pragma once

#include "mg_util_include.hpp"

#include <vector>
#include <string>

namespace mg_util {
// sets the value pointed to by val to the value in string if it is valid
// returns whether the string was a valid int
bool str2int(const std::string & str, int64_t * val);
bool str2double(const std::string & str, double * val);

std::vector<std::string> split(const std::string & str, char delim);
}

