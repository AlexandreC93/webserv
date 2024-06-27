#ifndef CGIEXECUTOR_HPP
#define CGIEXECUTOR_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

class CGIExecutor
{
public:
	CGIExecutor(const std::string &scriptPath, const std::map<std::string, std::string> &env);
	CGIExecutor(const CGIExecutor &other);				  // Constructeur par copie
	CGIExecutor &operator=(const CGIExecutor &other);	  // Opérateur d'affectation par copie
	~CGIExecutor();										  // Destructeur

	void execute() const;

private:
	std::string scriptPath_;
	std::map<std::string, std::string> env_;
};

#endif // CGIEXECUTOR_HPP
