#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "configParser.hpp"

class CGIHandler  // pas encore change
{
public:
	CGIHandler(const std::string &cgi_path, const std::string &cgi_script);
	Response handleCGIRequest(const Request &request);

private:
	std::string cgi_path;
	std::string cgi_script;
};

#endif // CGI_HANDLER_HPP
