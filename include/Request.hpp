#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

class Request { // pas encore change
public:
    std::string method;
    std::string uri;
    std::map<std::string, std::string> headers;
    std::string body;
	std::map<std::string, std::string> formData;

    void parse(const std::string& rawRequest);
	void parseMultipartFormData();
};

#endif
