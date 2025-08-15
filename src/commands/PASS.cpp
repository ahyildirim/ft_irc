#include "../../includes/Server.hpp"

void Server::handlePass(const std::string& password, Client& client)
{
	std::string trimmedPassword = (password[0] == ':') ? password.substr(1) : password;
	if (client.passCheck)
	{
		writeReply(client.cliFd, "You are already authenticated.\r\n");
		return;
	}

	if (trimmedPassword.empty())
	{
		writeReply(client.cliFd, "Password cannot be empty.\r\n");
		return;
	}

	if (trimmedPassword != this->_password)
	{
		writeReply(client.cliFd, "Wrong password.\r\n");
		return;
	}
	
	client.passCheck = true; // Password doğruysa, passCheck'i true yapar.
}