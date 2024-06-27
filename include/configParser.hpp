#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "Server.hpp"
#include "Launcher.hpp"

class ServerBlock;

class ConfigParser {
public:
	ConfigParser(const std::string path);
	ConfigParser(ConfigParser const &src);
	~ConfigParser();

	ConfigParser	&operator=(ConfigParser const &rhs);

	std::string                 getPath() const;
    std::vector<ServerBlock>    getServers() const;
    std::vector<std::string>    getServerConf() const;
    int                         getServerNb() const;

	void mainParse();
	// std::vector<ServerBlock> getServerBlocks() const;
	std::vector<ServerBlock> parseConfig();
	std::string cleanFile(std::string &content) const;
	std::string extractContent(std::string const &path);

	class ConfParserException : public std::exception {
            public :
                ConfParserException(std::string errMessage) throw() {
					_errMessage = "Parse Error: " + errMessage;
				}
				virtual const char* what() const throw() {
					return (_errMessage.c_str());
				}
				~ConfParserException() throw() {}
            
            private:
                std::string _errMessage;
        };

private:
	ConfigParser();
	std::string                 _path;
    std::vector<ServerBlock>    _servers;
    std::vector<std::string>	_serverConf;
    int 						_serverNb;
	static bool					_verbose;
};

#endif
