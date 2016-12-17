//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_TEXTPROCESSING_H
#define MAP_GEN_TEXTPROCESSING_H

#include "mg_util_global.hpp"

namespace mg_util {
// sets the value pointed to by val to the value in string if it is valid
// returns whether the string was a valid int
bool str2int(const string & str, int * val);
bool str2double(const string & str, double * val);

vector<string> split(const string & str, char delim);
}

#endif // MAP_GEN_TEXTPROCESSING_H
