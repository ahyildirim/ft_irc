SRCS += src/main.cpp \
		src/Server.cpp \
		src/Client.cpp \
		src/Channel.cpp \
		src/Utils.cpp \
		src/commands/JOIN.cpp \
		src/commands/KICK.cpp \
		src/commands/NICK.cpp \
		src/commands/PASS.cpp \
		src/commands/PRIVMSG.cpp \
		src/commands/QUIT.cpp \
		src/commands/TOPIC.cpp \
		src/commands/USER.cpp \
		src/commands/CAP.cpp \
		src/commands/INVITE.cpp \
		src/commands/MODE.cpp \
		src/commands/PING.cpp \
		src/commands/NAMES.cpp \

OBJS = $(SRCS:.cpp=.o)
NAME = ircserv
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
CXX = c++
RM = rm -f

ifdef DEBUG
	CXXFLAGS += -g -fsanitize=address
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

debug:
	$(CXX) -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address  -o $(NAME) $(SRCS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all