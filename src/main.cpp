#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h> // Pour waitpid

#define PORT 8081
#define MAX_CLIENTS 100

void handle_cgi(int client_fd, const char* script_path) {
    pid_t pid = fork();

    if (pid == 0) { // Enfant
        // Rediriger la sortie standard vers le client
        dup2(client_fd, STDOUT_FILENO);
        close(client_fd);

        // Configurer les variables d'environnement
        // setenv("REQUEST_METHOD", "GET", 1); // Par exemple
        // setenv("SCRIPT_NAME", script_path, 1);

        // Exécuter le script CGI
        execlp(script_path, script_path, NULL);
        // Si execlp échoue
        perror("execlp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent
        // Attendre que le processus enfant se termine
		std::cout << "wait for child process" << std::endl;
        waitpid(pid, NULL, 0); // Utiliser NULL au lieu de nullptr
    } else {
        perror("fork");
        close(client_fd);
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    } else {
		std::cout << "create socket ok" << std::endl;
	}

    // Associer le socket à une adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    } else {
		std::cout << "bind ok" << std::endl;
	}

    // Mettre le socket en mode écoute
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    } else {
		std::cout << "listen ok" << std::endl;
	}

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Initialiser poll
    memset(fds, 0, sizeof(fds));
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while (true) {
        int poll_count = poll(fds, nfds, -1);

        if (poll_count < 0) {
            perror("poll error");
            exit(EXIT_FAILURE);
        }

        // Vérifier les descripteurs prêts
        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents == 0)
                continue;

            // Nouveau client
            if (fds[i].fd == server_fd) {
                client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                if (client_fd < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                } else {
					std::cout << "new client" << std::endl;
				}

                fds[nfds].fd = client_fd;
                fds[nfds].events = POLLIN;
                nfds++;
                std::cout << "New connection, socket fd is " << client_fd << std::endl;
            } else {
                // Lire les données des clients existants
                char buffer[1024] = {0};
                int valread = read(fds[i].fd, buffer, 1024);
                if (valread == 0) {
                    // Déconnexion
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } else {
                    // Pour simplifier, on suppose que toute requête est une requête CGI
                    std::cout << "Received data: " << buffer << std::endl;
                    handle_cgi(fds[i].fd, "./index.html\n");
					std::cout << "end of data" << std::endl;
                    // Fermer la connexion après avoir traité le CGI
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
            }
        }
    }

    return 0;
}
