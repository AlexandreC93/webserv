#include "../include/utils.hpp"

std::string to_string(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

int string_to_int(const std::string& str) {
    std::istringstream iss(str);
    int value;
    iss >> value;
    return value;
}
