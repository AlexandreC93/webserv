NAME = webserv
CXX = g++
#WWW =  -Wall -Wextra -Werror $(WWW) -std=c++98
CXXFLAGS = -Wall -Wextra -Werror -lpthread 
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