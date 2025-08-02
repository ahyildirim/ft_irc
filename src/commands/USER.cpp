#include "../../includes/Server.hpp"

void Server::handleUser(const std::string& channelName, Client& client)
{
	if (!client.passCheck)
	{
		writeReply(client.cliFd, "You must authenticate first with PASS command.\r\n");
		return;
	}

	if (channelName.empty())
	{
		writeReply(client.cliFd, "Username cannot be empty.\r\n");
		return;
	}

	client.user = channelName; // Client'ın user'ı güncellenir.
	checkIfRegistered(client); // Client'ın kayıtlı olup olmadığını kontrol eder.
}