/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:39:29 by elie              #+#    #+#             */
/*   Updated: 2024/06/27 10:37:42 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "configParser.hpp"
#include "Server.hpp"
#include <fstream>

class ServerBlock;
class ConfigParser;

class LocationBlock {
public:
	LocationBlock();
	~LocationBlock();

	LocationBlock &operator=(LocationBlock const &C);

	// std::ifstream getFile() const;
	std::string getLocationPath() const;
	std::string getRoot() const;
	std::string getIndex() const;
	std::string getAllowMethods() const;
	std::string getAutoIndex() const;
	std::string getUploadStore() const;
	std::string getCgiPath() const;
	std::string getCgiExt() const;
	std::string getCgi() const;
	std::string getRet() const;

	// void setFile(std::ifstream file);
	void setLocationPath(std::string locationPath);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void setAllowMethods(std::string allowMethods);
	void setAutoIndex(std::string autoIndex);
	void setUploadStore(std::string uploadStore);
	void setCgiPath(std::string cgiPath);
	void setCgiExt(std::string cgiExt);
	void setCgi(std::string cgi);
	void setRet(std::string ret);
	void parseLocationBlock(std::istringstream &cleanedStream, ServerBlock &serverBlock);

private:
	// std::ifstream _file;
	std::string _location_path;
	std::string _root;
	std::string _index;
	std::string _allow_methods;
	std::string _autoindex;
	std::string _upload_store;
	std::string _cgi_path;
	std::string _cgi_ext;
	std::string _cgi;
	std::string _ret;
};

#endif