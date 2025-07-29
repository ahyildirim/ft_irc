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

		bool isOperator;
		bool isRegistered;
		bool passCheck;

		Client();
		~Client();

};

#endif