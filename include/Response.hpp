#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

class Response { // pas encore change
public:
    int status_code;
    std::map<std::string, std::string> headers;
    std::string body;

    std::string generate();

private:
    std::string get_status_message(int status_code);
};

#endif
