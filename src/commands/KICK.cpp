#include "../../includes/Server.hpp"

void Server::handleKick(const std::string& channelName, Client& client)
{
		(void)client; // Silinecek, warning i engellemek için kullanıldı.
	std::cout << "Channel: " << channelName << std::endl;
}