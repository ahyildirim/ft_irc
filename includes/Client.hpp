# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Client
{
	public:
		int cliFd;
		int port;

		char ipAddress[INET_ADDRSTRLEN];
		std::string buffer;

		std::string nickName;
		std::string user;
		std::string realName;
		std::string password;

		std::vector<std::string> messageBox;

		bool passCheck;
		bool isOperator;
		bool isRegistered;

		Client() : isOperator(0), isRegistered(0), passCheck(0), buffer("") {}
		~Client() {}

};

#endif