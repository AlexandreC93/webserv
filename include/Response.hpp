#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
public:
	int status_code;
	std::map<std::string, std::string> headers;
	std::string body;

	std::string Response::get_status_message(int status_code);
	std::string generate();
};

#endif
