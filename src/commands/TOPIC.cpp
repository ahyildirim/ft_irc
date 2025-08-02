#include "../../includes/Server.hpp"

void Server::handleTopic(const std::string& channelName, Client& client)
{
	(void)client; // Silinecek, warning i engellemek için kullanıldı.
	std::cout << "Channel: " << channelName << std::endl;
}