//
// Created by Arthur Eubanks on 24/12/15.
//

#ifndef MAPGEN_MG_UTIL_GLOBAL_H
#define MAPGEN_MG_UTIL_GLOBAL_H

#include <stdexcept>
#include <string>
#include <vector>

namespace mg_util {
using std::string;
using std::vector;
class mg_error : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};
}

#endif // MAPGEN_MG_UTIL_GLOBAL_H
