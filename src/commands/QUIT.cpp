#include "../../includes/Server.hpp"

void Server::handleQuit(const std::string& reason, Client& client)
{
	std::string quitMessage = ":" + client.nickName + " QUIT :" + reason + "\r\n";

	// Kanallara broadcast
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel& channel = it->second;
		if (channel.isClientInChannel(&client))
		{
			channel.removeClient(&client);
			channel.broadcastMessage(quitMessage, &client, *this);
		}
	}

	// FD’yi karşılıklı kapat (read ve write)
	shutdown(client.cliFd, SHUT_RDWR);

	client.toBeDisconnected = true;
}