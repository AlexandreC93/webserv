#include "../include/Server.hpp"
#include "../include/Request.hpp"
#include "../include/Response.hpp"
#include "../include/Handler.hpp"
#include "../include/utils.hpp"
#include "../include/CGIHandler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <poll.h>
#include <vector>
#include <cstdio>

ServerBlock::ServerBlock() {
    // if (Server::_verbose)
	// 	std::cout << "Server default constructor called" << std::endl;
	_socket = 0;
	_host = 0;
	_server_name = "";
	_root = "";
	_client_max_body_size = "";
	_index = "";
    _default_server = "";
	_error_page.clear();
	_listen = 0;
	// _timeout = -1;
}

ServerBlock::ServerBlock(ServerBlock const &src)
{
	_listen = src.getListen();
	_socket = src.getSocket();
	_path = src.getPath();
	_server_name = src.getServerName();
	_host = src.getHost();
	_root = src.getRoot();
	_index = src.getIndex();
	_default_server = src.getDefaultServer();
	_client_max_body_size = src.getClientMaxBodySize();
	_error_page = src.getErrorPage();
	_locations = src.getLocations();
}

ServerBlock::~ServerBlock() {}

ServerBlock &ServerBlock::operator=(ServerBlock const &C)
{
	if (this != &C)
	{
		_listen = C.getListen();
        _socket = C.getSocket();
		_path = C.getPath();
		_server_name = C.getServerName();
		_host = C.getHost();
		_root = C.getRoot();
		_index = C.getIndex();
		_default_server = C.getDefaultServer();
		_client_max_body_size = C.getClientMaxBodySize();
		_error_page = C.getErrorPage();
		_locations = C.getLocations();
	}
	return *this;
}

in_port_t ServerBlock::getListen() const{
	return _listen;
}

int	ServerBlock::getSocket() const {

	return (_socket);
}

struct sockaddr_in	ServerBlock::getSockAddr() const {

	return (_sockaddr);
}

std::string ServerBlock::getPath() const{
	return _path;
}

std::string ServerBlock::getServerName() const{
	return _server_name;
}

in_addr_t ServerBlock::getHost() const{
	return _host;
}

std::string ServerBlock::getRoot() const{
	return _root;
}

std::string ServerBlock::getIndex() const{
	return _index;
}

std::string ServerBlock::getDefaultServer() const{
	return _default_server;
}

std::string ServerBlock::getClientMaxBodySize() const{
	return _client_max_body_size;
}

std::map<int, std::string> ServerBlock::getErrorPage() const{
	return _error_page;
}

std::vector<LocationBlock>& ServerBlock::getLocations(){
	return _locations;
}

const std::vector<LocationBlock>& ServerBlock::getLocations() const{
	return _locations;
}

void ServerBlock::setListen(in_port_t listen){
	_listen = listen;
}

void	ServerBlock::setSocket(int socket)
{
	_socket = socket;
}

void	ServerBlock::setSockaddr()
{
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_addr.s_addr = _host;
	_sockaddr.sin_port = _listen;
}

void ServerBlock::setPath(std::string path){
	_path = path;
}

void ServerBlock::setServerName(std::string serverName){
	_server_name = serverName;
}

void ServerBlock::setHost(in_addr_t host){
	_host = host;
}

void ServerBlock::setRoot(std::string root){
	_root = root;
}

void ServerBlock::setIndex(std::string index){
	_index = index;
}

void ServerBlock::setDefaultServer(std::string defaultServer){
	_default_server = defaultServer;
}

void ServerBlock::setClientMaxBodySize(std::string clientMaxBodySize){
	_client_max_body_size = clientMaxBodySize;
}

void ServerBlock::setErrorPage(std::map<int, std::string> errorPage)
{
	if (errorPage.empty() == true)
		return ;
	_error_page = errorPage;
}

void ServerBlock::setLocations(std::vector<LocationBlock> location){
	_locations = location;
}

// ServerBlock::ServerBlock(const ServerBlock &config) : serverBlock(config) {}

void	ServerBlock::bind_server()
{
	if (bind(this->_socket, (struct sockaddr *)&(this->_sockaddr), sizeof(this->_sockaddr)) < 0)
		throw ServerException("bind: " + static_cast<std::string>(strerror(errno)));
}

void ServerBlock::start()
{

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_listen);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << _listen << std::endl;
    handleConnections();
}

void ServerBlock::handleConnections()
{
    const int MAX_CLIENTS = 100;
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while (true)
    {
        int ret = poll(fds, nfds, -1);

        if (ret < 0)
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; i++)
        {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == server_fd)
                {
                    int new_socket = accept(server_fd, NULL, NULL);
                    if (new_socket < 0)
                    {
                        perror("accept");
                        continue;
                    }
                    std::cout << "New connection accepted, fd: " << new_socket << std::endl;
                    fds[nfds].fd = new_socket;
                    fds[nfds].events = POLLIN;
                    nfds++;
                }
                else
                {
                    char buffer[4096] = {0};
                    int valread = read(fds[i].fd, buffer, 4096);
                    if (valread < 0)
                    {
                        perror("read");
                        close(fds[i].fd);
                        fds[i] = fds[--nfds];
                    }
                    else if (valread == 0)
                    {
                        std::cout << "Connection closed, fd: " << fds[i].fd << std::endl;
                        close(fds[i].fd);
                        fds[i] = fds[--nfds];
                    }
                    else
                    {
                        std::string rawRequest(buffer, valread);
                        Request request;
                        request.parse(rawRequest);

                        Response response;

                        bool isCGIRequest = false;
                        std::string cgiPath, cgiScript;

                        // Determine if the request should be handled by CGI
                        for (std::vector<LocationBlock>::const_iterator it = _locations.begin(); it != _locations.end(); ++it) {
                            const LocationBlock& location = *it;
                            if (request.uri.find(location.getLocationPath()) == 0) {
                                if (!location.getCgiPath().empty()) {
                                    cgiPath = location.getCgiPath();
                                    cgiScript = request.uri.substr(location.getLocationPath().length());
                                    break;
                                }
                            }
                        }

                        if (isCGIRequest)
                        {
                            CGIHandler cgiHandler(cgiPath, cgiScript);
                            response = cgiHandler.handleCGIRequest(request);
                        }
                        else
                        {
                            if (request.method == "GET")
                            {
                                std::string responseContent = handleGetRequest(*this, request.uri);
                                response.status_code = 200;
                                response.body = responseContent;
                                response.headers["Content-Type"] = "text/html";
                                response.headers["Content-Length"] = to_string(responseContent.length());
                            }
                            else if (request.method == "POST")
                            {
                                std::string responseContent = handlePostRequest(*this, request);
                                response.status_code = 200;
                                response.body = responseContent;
                                response.headers["Content-Type"] = "text/html";
                                response.headers["Content-Length"] = to_string(responseContent.length());
                            }
                            else if (request.method == "DELETE")
                            {
                                std::string responseContent = handleDeleteRequest(request.uri);
                                if (responseContent == "204 No Content")
                                {
                                    response.status_code = 204;
                                    response.body = "";
                                }
                                else
                                {
                                    response.status_code = 404;
                                    response.body = responseContent;
                                }
                                response.headers["Content-Type"] = "text/html";
                                response.headers["Content-Length"] = to_string(response.body.length());
                            }
                            else
                            {
                                response.status_code = 405;
                                response.body = "405 - Method Not Allowed";
                                response.headers["Content-Type"] = "text/html";
                                response.headers["Content-Length"] = to_string(response.body.length());
                            }
                        }

                        std::string httpResponse = response.generate();
                        send(fds[i].fd, httpResponse.c_str(), httpResponse.length(), 0);
                        std::cout << "Response sent to fd: " << fds[i].fd << std::endl;
                    }
                }
            }
        }
    }
}

// void ServerBlock::parseServerBlock(std::istringstream &cleanedStream)
// {
//     std::string token;
//     while (cleanedStream >> token && token != "}") {
//         if (token == "listen") {
//             cleanedStream >> _listen;
//         } else if (token == "server_name") {
//             std::getline(cleanedStream >> std::ws, _server_name, ';');
//         } else if (token == "host") {
//             std::string hostValue;
//             cleanedStream >> hostValue;
//             _host = inet_addr(hostValue.c_str()); 
//         } else if (token == "root") {
//             std::getline(cleanedStream >> std::ws, _root, ';');
//         } else if (token == "index") {
//             std::getline(cleanedStream >> std::ws, _index, ';');
//         } else if (token == "error_page") {
//             std::string error_info;
//             std::getline(cleanedStream >> std::ws, error_info, ';');
//             std::istringstream error_stream(error_info);
//             int error_code;
//             std::string error_path;
//             error_stream >> error_code >> error_path;
//             _error_page[error_code] = error_path;
//         } else if (token == "client_max_body_size") {
//             std::getline(cleanedStream >> std::ws, _client_max_body_size, ';');
//         } else if (token == "default_server") {
//             std::getline(cleanedStream >> std::ws, _default_server, ';');
//         } else if (token == "location") {
//             LocationBlock locationBlock;
//             locationBlock.parseLocationBlock(cleanedStream, *this);
//         } else {
//             std::cout << "Unknown directive: " << token << std::endl;
//             // Gérer les directives inconnues si nécessaire
//         }
//     }
// }



void ServerBlock::parseServerBlock(std::istringstream &cleanedStream)
{
    std::string token;
    while (cleanedStream >> token && token != "}") {
        if (token == "listen")
            cleanedStream >> _listen;
        else if (token == "server_name")
            std::getline(cleanedStream >> std::ws, _server_name, ';');
		else if (token == "host")
		{
            std::string hostValue;
            cleanedStream >> hostValue;
            _host = inet_addr(hostValue.c_str()); 
        }
		else if (token == "root")
            std::getline(cleanedStream >> std::ws, _root, ';');
        else if (token == "index")
            std::getline(cleanedStream >> std::ws, _index, ';');
        else if (token == "client_max_body_size")
            std::getline(cleanedStream >> std::ws, _client_max_body_size, ';');
        else if (token == "default_server")
            std::getline(cleanedStream >> std::ws, _default_server, ';');
        else if (token == "error_page")
		{
            std::string error_info;
            std::getline(cleanedStream >> std::ws, error_info, ';');
            std::istringstream error_stream(error_info);
            int error_code;
            std::string error_path;
            error_stream >> error_code;
            error_stream >> error_path;
            _error_page[error_code] = error_path;
        }
		else if (token == "location")
		{
            LocationBlock locationBlock;
            locationBlock.parseLocationBlock(cleanedStream, *this);
        }
    }
}
