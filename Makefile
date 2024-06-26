NAME = webserv
CXX = g++
#WWW =  -Wall -Wextra -Werror $(WWW)
CXXFLAGS = -Wall -Wextra -Werror -lpthread -std=c++98
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
INCLUDES = -I include

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX)  $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
