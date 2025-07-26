# ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <stdlib.h>
# include <vector>
# include <netinet/in.h>

class Server 
{
	private:
		int _port;
		std::string _password;
		int _server_socket_fd;
		std::vector<int> _client_fds;
		char _buffer[1024];
	public:
		Server(int port, const std::string &password);
};

#endif