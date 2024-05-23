#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

class Request {
public:
    std::string method;
    std::string uri;
    std::map<std::string, std::string> headers;
    std::string body;

    void parse(const std::string& rawRequest);
};

#endif
