#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

#define PORT 8080

void handle_client(int client_fd)
{
	char buffer[1024];
	int valread = read(client_fd, buffer, 1024);

	if (valread > 0)
	{
		buffer[valread] = '\0';
		std::istringstream request(buffer);
		std::string method, path, version;
		request >> method >> path >> version;

		if (method == "GET")
		{
			std::string file_path = path;
			if (path == "/")
				file_path = "./www/html/index.html";
		}
	}
}

int main(int ac, char **av)
{
	int server_fd, client_fd;
	struct sockaddr_in address;
	int opt = 1;
	int addr_len = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Server is listening on port " << PORT << std::endl;

	while (true)
	{
		if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addr_len)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		handle_client(client_fd);
	}
	return (0);
}
