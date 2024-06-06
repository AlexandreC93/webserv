#include "CGIExecutor.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>

// Constructeur
CGIExecutor::CGIExecutor(const std::string &scriptPath, const std::map<std::string, std::string> &env)
    : scriptPath_(scriptPath), env_(env) {}

// Constructeur par copie
CGIExecutor::CGIExecutor(const CGIExecutor &other)
    : scriptPath_(other.scriptPath_), env_(other.env_) {}

// Constructeur par déplacement
CGIExecutor::CGIExecutor(CGIExecutor &&other) noexcept
    : scriptPath_(std::move(other.scriptPath_)), env_(std::move(other.env_)) {}

// Opérateur d'affectation par copie
CGIExecutor &CGIExecutor::operator=(const CGIExecutor &other)
{
    if (this != &other)
    {
        scriptPath_ = other.scriptPath_;
        env_ = other.env_;
    }
    return *this;
}

// Opérateur d'affectation par déplacement
CGIExecutor &CGIExecutor::operator=(CGIExecutor &&other) noexcept
{
    if (this != &other)
    {
        scriptPath_ = std::move(other.scriptPath_);
        env_ = std::move(other.env_);
    }
    return *this;
}

// Destructeur
CGIExecutor::~CGIExecutor() {}

// Méthode pour exécuter le script CGI
void CGIExecutor::execute() const
{
    pid_t pid = fork();

    if (pid < 0)
    {
        // Gestion de l'erreur de fork
        std::cerr << "Error: Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Enfant : Exécuter le script CGI
        std::vector<char *> envp;
        for (const auto &pair : env_)
        {
            std::string envEntry = pair.first + "=" + pair.second;
            envp.push_back(const_cast<char *>(envEntry.c_str()));
        }
        envp.push_back(nullptr);

        int scriptFD = open(scriptPath_.c_str(), O_RDONLY);
        if (scriptFD < 0)
        {
            std::cerr << "Error: Could not open script file" << std::endl;
            exit(EXIT_FAILURE);
        }

        dup2(scriptFD, STDIN_FILENO);
        close(scriptFD);

        int cgiOutput[2];
        pipe(cgiOutput);

        if (fork() == 0)
        {
            dup2(cgiOutput[1], STDOUT_FILENO);
            close(cgiOutput[0]);
            close(cgiOutput[1]);

            execl(scriptPath_.c_str(), scriptPath_.c_str(), nullptr);
            std::cerr << "Error: execl failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        close(cgiOutput[1]);

        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(cgiOutput[0], buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        close(cgiOutput[0]);

        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent : Attendre la fin du processus enfant
        int status;
        waitpid(pid, &status, 0);
    }
}

// #include <iostream>
// #include <string>
// #include <map>
// #include <vector>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <fcntl.h>
// #include <cstdlib>

// // Fonction pour exécuter un script CGI
// void executeCGI(const std::string& scriptPath, const std::map<std::string, std::string>& env) {
//     pid_t pid = fork();
    
//     if (pid < 0) {
//         // Gestion de l'erreur de fork
//         std::cerr << "Error: Fork failed" << std::endl;
//         exit(EXIT_FAILURE);
//     } else if (pid == 0) {
//         // Enfant : Exécuter le script CGI
//         std::vector<char*> envp;
//         for (const auto& pair : env) {
//             std::string envEntry = pair.first + "=" + pair.second;
//             envp.push_back(const_cast<char*>(envEntry.c_str()));
//         }
//         envp.push_back(nullptr);

//         int scriptFD = open(scriptPath.c_str(), O_RDONLY);
//         if (scriptFD < 0) {
//             std::cerr << "Error: Could not open script file" << std::endl;
//             exit(EXIT_FAILURE);
//         }

//         dup2(scriptFD, STDIN_FILENO);
//         close(scriptFD);

//         int cgiOutput[2];
//         pipe(cgiOutput);

//         if (fork() == 0) {
//             dup2(cgiOutput[1], STDOUT_FILENO);
//             close(cgiOutput[0]);
//             close(cgiOutput[1]);

//             execl(scriptPath.c_str(), scriptPath.c_str(), nullptr);
//             std::cerr << "Error: execl failed" << std::endl;
//             exit(EXIT_FAILURE);
//         }

//         close(cgiOutput[1]);
        
//         char buffer[1024];
//         ssize_t bytesRead;
//         while ((bytesRead = read(cgiOutput[0], buffer, sizeof(buffer))) > 0) {
//             write(STDOUT_FILENO, buffer, bytesRead);
//         }
//         close(cgiOutput[0]);

//         exit(EXIT_SUCCESS);
//     } else {
//         // Parent : Attendre la fin du processus enfant
//         int status;
//         waitpid(pid, &status, 0);
//     }
// }

// int main() {
//     // Configuration de l'environnement CGI
//     std::map<std::string, std::string> env;
//     env["REQUEST_METHOD"] = "GET";
//     env["SCRIPT_FILENAME"] = "../cgi-bin/cgi-script.php";
//     env["QUERY_STRING"] = "name=JohnDoe";

//     // Appeler le script CGI
//     executeCGI("../cgi-bin/cgi-script.php", env);

//     return 0;
// }
