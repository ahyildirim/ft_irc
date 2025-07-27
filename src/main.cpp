#include "../includes/Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
	else if (checkPort(av[1]))
		std::cerr << "Invalid port number." << std::endl;
	else
		Server server = Server(atoi(av[1]), av[2]); // Server sınıfının constructor'ını çağır
	return (0);
}