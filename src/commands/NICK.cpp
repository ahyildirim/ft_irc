#include "../../includes/Server.hpp"

void Server::handleNick(const std::string& nickName, Client& client)
{
	if (!client.passCheck)
	{
		writeReply(client.cliFd, ERR_NOTREGISTERED(client.nickName));
		return;
	}

	if (nickName.empty())
	{
		writeReply(client.cliFd, ERR_NONICKNAMEGIVEN(client.nickName));
		return;
	}

	for (std::map<int, Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (it->second.nickName == nickName)
		{
			writeReply(client.cliFd, ERR_NICKNAMEINUSE(client.nickName, nickName));
			return;
		}
	}

	client.nickName = nickName; // Client'ın nickName'i güncellenir.
	checkIfRegistered(client); // Client'ın kayıtlı olup olmadığını kontrol eder.
}