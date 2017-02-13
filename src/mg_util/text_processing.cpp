//
// Created by Arthur Eubanks on 20/12/15.
//

#include "text_processing.hpp"

#include <sstream>

#include "mg_log/Log.hpp"

namespace mg_util {
bool str2int(const std::string & str, int64_t * val) {
    if (str.empty()) {
        return false;
    }

    char * end;
    const char * c_str = str.c_str();
    errno = 0;
    auto ret = strtol(c_str, &end, 0);
    if (errno == ERANGE) {
        mg_log::error("number out of range: ", str);
        return false;
    }

    bool valid = end == c_str + str.length();
    if (valid) {
        *val = ret;
    }
    return valid;
}

bool str2double(const std::string & str, double * val) {
    if (str.empty()) {
        return false;
    }

    char * end;
    const char * c_str = str.c_str();
    errno = 0;
    auto ret = strtod(c_str, &end);
    if (errno == ERANGE) {
        mg_log::error("number out of range: ", str);
        return false;
    }

    bool valid = end == c_str + str.length();
    if (valid) {
        *val = ret;
    }
    return valid;
}

std::vector<std::string> split(const std::string & str, char delim) {
    std::vector<std::string> ret;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim)) {
        ret.push_back(item);
    }
    return ret;
}
}
