#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

namespace Logger {
    inline void info(const std::string& msg) {
        std::cout << "[BACKEND INFO] " << msg << std::endl;
    }

    inline void debug(const std::string& msg) {
        std::cout << "[BACKEND DEBUG] " << msg << std::endl;
    }

    inline void warn(const std::string& msg) {
        std::cout << "[BACKEND WARN] " << msg << std::endl;
    }
};

#endif