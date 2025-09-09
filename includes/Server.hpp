# ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <cstdlib>
# include <vector>
# include <netinet/in.h>
# include <poll.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <map>
# include <csignal>
# include <cerrno>
# include "Client.hpp"
# include "Utils.hpp"
# include "Channel.hpp"
# include "Replies.hpp"

class Client;
class Channel;

extern volatile sig_atomic_t g_running;

class Server 
{
	private:
		int _port;
		std::string _password;
		int _server_socket_fd;
		char _buffer[1024];
		int _reuse;
		std::map<int, Client> clients;
		std::map<std::string, Channel> _channels;

	public:
		std::vector<pollfd> pollfds;

		struct Command{
			std::string cmd;
			void (Server::*handler)(const std::string& args, Client& client);
		};

		static const Command commandTable[];

		Server(int port, const std::string &password);
		~Server();

		void handleCommand(Client &client, const std::string &command);
		void checkIfRegistered(Client &client);

		Channel* findChannel(const std::string& channelName);
		Client* findClientByNick(const std::string &nick);

		//Commands
		void handlePass(const std::string& password, Client& client);
		void handleNick(const std::string& nickName, Client& client);
		void handleUser(const std::string& userName, Client& client);
		void handleJoin(const std::string& arg, Client& client);
		void handlePrivmsg(const std::string& arg, Client& client);
		void handleQuit(const std::string& reason, Client& client);
		void handleTopic(const std::string& arg, Client& client);
		void handleKick(const std::string& channelName, Client& client);
		void handleCap(const std::string& channelName, Client& client);
		void handleInvite(const std::string& arg, Client& client);
		void handleMode(const std::string& arg, Client& client);
		void handlePing(const std::string& arg, Client& client);
		void handleNames(const std::string& arg, Client& client);


		
};

#endif