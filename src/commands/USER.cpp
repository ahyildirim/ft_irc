#include "../../includes/Server.hpp"

void Server::handleUser(const std::string& userName, Client& client)
{
	if (!client.passCheck)
	{
		writeReply(client.cliFd, "You must authenticate first with PASS command.\r\n");
		return;
	}

	if (userName.empty())
	{
		writeReply(client.cliFd, "Username cannot be empty.\r\n");
		return;
	}

	client.user = userName; // Client'ın user'ı güncellenir.
	checkIfRegistered(client); // Client'ın kayıtlı olup olmadığını kontrol eder.
}