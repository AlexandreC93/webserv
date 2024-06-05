#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include "Request.hpp"
#include "utils.hpp"
#include "configParser.hpp"

std::string handleGetRequest(ServerBlock &serverBlock, const std::string &uri);
std::string handlePostRequest(ServerBlock &serverBlock, const Request &request);
std::string handleDeleteRequest(const std::string &uri);
std::string saveUploadedFile(const Request &request, const std::string &uploadDir);
// std::string 

#endif
