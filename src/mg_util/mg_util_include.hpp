#pragma once

#include "global_include.hpp"

#include <stdexcept>

namespace mg_util {
class mg_error : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};
}
