#include "../../includes/Server.hpp"

void Server::handleUser(const std::string& userName, Client& client)
{
	if (!client.passCheck)
	{
		writeReply(client.cliFd, ERR_NOTREGISTERED(client.nickName)); // RPL_NOTREGISTERED
		return;
	}

	if (userName.empty())
	{
		writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "USER")); // ERR_NEEDMOREPARAMS
		return;
	}

	client.user = userName; // Client'ın user'ı güncellenir.
	checkIfRegistered(client); // Client'ın kayıtlı olup olmadığını kontrol eder.
}