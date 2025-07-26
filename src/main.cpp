#include "../includes/Server.hpp"

int checkPort(const char *port)
{
	for (int i = 0; port[i]; i++) //tüm string boyunca sayılara bak, sayı değilse hata
		if (!isdigit(port[i]))
			return (1);
	
	int p = atoi(port);
	if (p < 1024 || p > 65535) // port numarası 1024 ile 65535 arasında olmalı
		return (1);
	return (0);
}

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