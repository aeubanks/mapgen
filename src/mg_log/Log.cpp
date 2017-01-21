#include "Log.hpp"

#include <iostream>
#include <fstream>

namespace mg_log {

std::ostream * os = nullptr;

LogInit::LogInit(): allocated(false) {
    if (os) {
        error("cannot reset log output");
    }
    os = &std::cout;
}

LogInit::LogInit(const std::string & file_name): allocated(true) {
    if (os) {
        error("cannot reset log output");
    }
    os = new std::ofstream(file_name);
}

LogInit::LogInit(const char * file_name): allocated(true) {
    if (os) {
        error("cannot reset log output");
    }
    os = new std::ofstream(file_name);
}

LogInit::~LogInit() {
    if (allocated) {
        delete os;
    }
}

}
