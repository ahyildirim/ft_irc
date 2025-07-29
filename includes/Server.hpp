# ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <stdlib.h>
# include <vector>
# include <netinet/in.h>
# include <poll.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include "Client.hpp"
# include "Utils.hpp"

class Client;

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

	public:
		Server(int port, const std::string &password);
		void handleCommand(Client &client, const std::string &command);
};

#endif