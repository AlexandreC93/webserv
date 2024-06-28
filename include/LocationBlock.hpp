/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenibart <lenibart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:39:29 by elie              #+#    #+#             */
/*   Updated: 2024/06/28 14:07:49 by lenibart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include "configParser.hpp"
#include "Server.hpp"
#include <fstream>

class ServerBlock;
class ConfigParser;

class LocationBlock {
public:
	LocationBlock();
	LocationBlock(LocationBlock const &src);
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

	class LocationException : public std::exception {
            public :
                LocationException(std::string errMessage) throw() {
					_errMessage = "Location Error: " + errMessage;
				}
				virtual const char* what() const throw() {
					return (_errMessage.c_str());
				}
				~LocationException() throw() {}
            
            private:
                std::string _errMessage;
        };

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