#ifndef CONFIG_PARSER2_HPP
#define CONFIG_PARSER2_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <sstream>

class ConfigParser
{
public:
	ConfigParser(std::string path);
	ConfigParser &operator=(ConfigParser const &rhs);
	~ConfigParser();
	class ConfParserException : public std::exception
	{
	public:
		ConfParserException(std::string errMessage) throw()
		{
			_errMessage = "Parse Error: " + errMessage;
		}
		virtual const char *what() const throw()
		{
			return (_errMessage.c_str());
		}
		~ConfParserException() throw() {}

	private:
		std::string _errMessage;
	};

private:
	ConfigParser();
	std::ifstream file;
	std::string filename;
	std::vector<Server> serverBlocks;

	void parseServerBlock();
	void parseLocationBlock(Server &serverBlock);
	std::string getNextToken();
};

#endif