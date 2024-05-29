
#ifndef CONFIGLOCATION_HPP
# define CONFIGLOCATION_HPP

#include <iostream>
#include <netinet/in.h>
// # include <fstream>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
// # include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <cstdarg>
// #include containers
#include <vector>
#include <map>
#include <algorithm>

class Location
{
	public:
		Location();
		Location(Location const &C);
		~Location();

		Location &operator=(Location const &C);

		std::string getFilePath() const;
		std::string getRoot() const;
		std::vector<std::string> getIdxFiles() const;

		void setFilePath(std::string file_path);
		void setRoot(std::string root, std::string server_root);
		void setIdxFiles(std::vector<std::string> idx_files);

	private:
		std::string file_path;
		std::string root;
		std::vector<std::string> idx_files;
};

#endif
