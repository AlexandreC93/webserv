#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include "Request.hpp"
#include "utils.hpp"

std::string handleGetRequest(const std::string &uri);
std::string handlePostRequest(const Request &request);
std::string handleDeleteRequest(const std::string &uri);
std::string saveUploadedFile(const Request &request, const std::string &uploadDir);

#endif
