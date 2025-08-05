#include "../includes/Server.hpp"

volatile sig_atomic_t g_running = 1;

void signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
	{
		g_running = 0; // Global değişkeni 0 yaparak döngüyü durdur.
		std::cout << "\nServer shutting down..." << std::endl;
	}
}

int main(int ac, char **av)
{
	std::signal(SIGINT, signalHandler); // SIGINT (Ctrl+C) ve SIGTERM sinyallerini yakalamak için signalHandler fonksiyonunu kullanır.
	std::signal(SIGTERM, signalHandler); // SIGTERM sinyali için de aynı handler

	if (ac != 3)
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
	else if (checkPort(av[1]))
		std::cerr << "Invalid port number." << std::endl;
	else
		Server server = Server(atoi(av[1]), av[2]); // Server sınıfının constructor'ını çağır
	return (0);
}