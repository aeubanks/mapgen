#include "sdl2_include.hpp"

#include <sstream>

static std::string create_sdl2_error_str(std::string err, const char * (*get_error)()) {
    if (!get_error) {
        return err;
    }
    std::stringstream ss;
    ss << err << '(' << get_error() << ')';
    return ss.str();
}

namespace sdl2 {
sdl2_error::sdl2_error(std::string err, const char * (*get_error)())
    : std::runtime_error(create_sdl2_error_str(std::move(err), get_error)) {}
}
