#include "../include/CGIHandler.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <iostream>

CGIHandler::CGIHandler(const std::string &cgi_path, const std::string &cgi_script)
	: cgi_path(cgi_path), cgi_script(cgi_script) {}

Response CGIHandler::handleCGIRequest(const Request &request)
{
	int pipe_in[2], pipe_out[2];
	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDOUT_FILENO);

		close(pipe_in[1]);
		close(pipe_out[0]);

		execl(cgi_path.c_str(), cgi_script.c_str(), NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipe_in[0]);
		close(pipe_out[1]);

		write(pipe_in[1], request.body.c_str(), request.body.length());
		close(pipe_in[1]);

		char buffer[4096];
		std::ostringstream responseStream;
		int bytesRead;
		while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer))) > 0)
		{
			responseStream.write(buffer, bytesRead);
		}
		close(pipe_out[0]);

		int status;
		waitpid(pid, &status, 0);

		Response response;
		response.status_code = 200;
		response.body = responseStream.str();
		response.headers["Content-Type"] = "text/html";
		response.headers["Content-Length"] = std::to_string(response.body.length());
		return response;
	}
}
