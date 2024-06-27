/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenibart <lenibart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:40:52 by elie              #+#    #+#             */
/*   Updated: 2024/06/27 19:36:06 by lenibart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/LocationBlock.hpp"

LocationBlock::LocationBlock() {
	_location_path = "";
	_root = "";
	_index = "";
	_allow_methods = "";
	_autoindex = "";
	_upload_store = "";
	_cgi_path = "";
	_cgi_ext = "";
	_cgi = "";
	_ret = "";
}
LocationBlock::~LocationBlock() {}

LocationBlock &LocationBlock::operator=(LocationBlock const &C)
{
	if (this != &C)
	{
		// _file = C.getFile();
		_location_path = C.getLocationPath();
		_root = C.getRoot();
		_index = C.getIndex();
		_allow_methods = C.getAllowMethods();
		_autoindex = C.getAutoIndex();
		_upload_store = C.getUploadStore();
		_cgi_path = C.getCgiPath();
		_cgi_ext = C.getCgiExt();
		_cgi = C.getCgi();
		_ret = C.getRet();
	}
	return *this;
}

std::string LocationBlock::getLocationPath() const{
	return _location_path;
}

std::string LocationBlock::getRoot() const{
	return _root;
}

std::string LocationBlock::getIndex() const{
	return _index;
}

std::string LocationBlock::getAllowMethods() const{
	return _allow_methods;
}

std::string LocationBlock::getAutoIndex() const{
	return _autoindex;
}

std::string LocationBlock::getUploadStore() const{
	return _upload_store;
}

std::string LocationBlock::getCgiPath() const{
	return _cgi_path;
}

std::string LocationBlock::getCgiExt() const{
	return _cgi_ext;
}

std::string LocationBlock::getCgi() const{
	return _cgi;
}

std::string LocationBlock::getRet() const{
	return _ret;
}

void LocationBlock::setLocationPath(std::string locationPath){
	_location_path = locationPath;
}

void LocationBlock::setRoot(std::string root){
	_root = root;
}

void LocationBlock::setIndex(std::string index){
	_index = index;
}

void LocationBlock::setAllowMethods(std::string allowMethods){
	_allow_methods = allowMethods;
}

void LocationBlock::setAutoIndex(std::string autoIndex){
	_autoindex = autoIndex;
}

void LocationBlock::setUploadStore(std::string uploadStore){
	_upload_store = uploadStore;
}

void LocationBlock::setCgiPath(std::string cgiPath){
	_cgi_path = cgiPath;
}

void LocationBlock::setCgiExt(std::string cgiExt){
	_cgi_ext = cgiExt;
}

void LocationBlock::setCgi(std::string cgi){
	_cgi = cgi;
}

void LocationBlock::setRet(std::string ret){
	_ret = ret;
}

void LocationBlock::parseLocationBlock(std::istringstream &cleanedStream, ServerBlock &serverBlock)
{
    std::string token;
    std::getline(cleanedStream >> std::ws, _location_path, '{'); // Récupère le chemin spécifique
    while (cleanedStream >> token) {
        if (token == "}") {
            break;
        }
        if (token == "root") {
            std::getline(cleanedStream >> std::ws, _root, ';');
        } else if (token == "index") {
            std::getline(cleanedStream >> std::ws, _index, ';');
        } else if (token == "allow_methods" || token == "methods") {
            std::getline(cleanedStream >> std::ws, _allow_methods, ';');
        } else if (token == "autoindex") {
            std::getline(cleanedStream >> std::ws, _autoindex, ';');
        } else if (token == "upload_store") {
            std::getline(cleanedStream >> std::ws, _upload_store, ';');
        } else if (token == "cgi_path") {
            std::getline(cleanedStream >> std::ws, _cgi_path, ';');
        } else if (token == "cgi_ext") {
            std::getline(cleanedStream >> std::ws, _cgi_ext, ';');
        } else if (token == "cgi") {
            std::getline(cleanedStream >> std::ws, _cgi, ';');
        } else if (token == "return") {
            std::getline(cleanedStream >> std::ws, _ret, ';');
        }
    }
    serverBlock.getLocations().push_back(*this);
}