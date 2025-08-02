#include "../../includes/Server.hpp"

void Server::handlePass(const std::string& password, Client& client)
{
	if (client.passCheck)
	{
		writeReply(client.cliFd, "You are already authenticated.\r\n");
		return;
	}

	if (password.empty())
	{
		writeReply(client.cliFd, "Password cannot be empty.\r\n");
		return;
	}

	if (password != this->_password)
	{
		writeReply(client.cliFd, "Wrong password.\r\n");
		return;
	}
	
	client.passCheck = true; // Password doğruysa, passCheck'i true yapar.
}