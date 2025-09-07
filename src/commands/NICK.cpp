#include "../../includes/Server.hpp"

void Server::handleNick(const std::string& nickName, Client& client)
{
	if (!client.passCheck)
	{
		writeReply(client.cliFd, "You must authenticate first with PASS command.\r\n");
		return;
	}

	if (nickName.empty())
	{
		writeReply(client.cliFd, "Nickname cannot be empty.\r\n");
		return;
	}

	for (std::map<int, Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (it->second.nickName == nickName)
		{
			writeReply(client.cliFd, "Nickname already in use.\r\n");
			return;
		}
	}

	client.nickName = nickName; // Client'ın nickName'i güncellenir.
	checkIfRegistered(client); // Client'ın kayıtlı olup olmadığını kontrol eder.
}