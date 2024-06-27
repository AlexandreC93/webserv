#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <string>
#include <vector>

class LocationConfig {
public:
    std::string path;
    std::string root;
    std::vector<std::string> index_files;
    std::string cgi_path;
    std::vector<std::string> methods;

    void parseLocationConfig(const std::string& configBlock);
};

#endif
