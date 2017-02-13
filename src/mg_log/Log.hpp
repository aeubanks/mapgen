#pragma once

#include <ostream>

namespace mg_log {
extern std::ostream * os;

template<typename... Ts>
void info(Ts... ts) {
    (*os << ... << ts);
    //((*os << ts), ...);
    /*
    auto i = {(*os << ts, 0)...};
    (void) i;
    */
    *os << '\n';
}

template<typename... Ts>
void error(Ts... ts) {
    (*os << ... << ts);
    //((*os << ts), ...);
    /*
    auto i = {(*os << ts, 0)...};
    (void) i;
    */
    *os << '\n';
}

class LogInit {
public:
    // log to stdout
    LogInit();
    // log to file
    LogInit(const std::string & file_name);
    LogInit(const char * file_name);

    LogInit(LogInit &&) = delete;
    LogInit(const LogInit &) = delete;
    LogInit & operator=(const LogInit &) = delete;
    LogInit & operator=(LogInit &&) = delete;

    ~LogInit();

private:
    bool allocated;
};

}

