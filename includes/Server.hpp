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
# include "Client.hpp"
# include "Utils.hpp"
# include "Channel.hpp"

class Client;
class Channel;

class Server 
{
	private:
		int _port;
		std::string _password;
		int _server_socket_fd;
		std::vector<int> _client_fds;
		char _buffer[1024];
		std::vector<Client> clients;
		int _reuse;
		std::map<std::string, Channel> _channels;

	public:
		struct Command{
			std::string cmd;
			void (Server::*handler)(const std::string& args, Client& client); //içeriğe arg, client eklenecek
		};

		static const Command commandTable[];

		Server(int port, const std::string &password);
		void handleCommand(Client &client, const std::string &command);
		void checkIfRegistered(Client &client);

		//Commands
		void handlePass(const std::string& password, Client& client);
		void handleNick(const std::string& nickName, Client& client);
		void handleUser(const std::string& channelName, Client& client);
		void handleJoin(const std::string& channelName, Client& client);
		void handlePrivmsg(const std::string& channelName, Client& client);
		void handleQuit(const std::string& channelName, Client& client);
		void handleTopic(const std::string& channelName, Client& client);
		void handleKick(const std::string& channelName, Client& client);
		void handleCap(const std::string& channelName, Client& client);
};

#endif