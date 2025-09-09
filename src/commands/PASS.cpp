#include "../../includes/Server.hpp"

void Server::handlePass(const std::string& password, Client& client)
{
	std::string trimmedPassword = (password[0] == ':') ? password.substr(1) : password;
	if (client.passCheck)
	{
		writeReply(client.cliFd, ERR_ALREADYREGISTRED(client.nickName));
		return;
	}

	if ((trimmedPassword.empty()) || trimmedPassword != this->_password)
	{
		writeReply(client.cliFd, ERR_PASSWDMISMATCH(client.nickName));
		return;
	}
	
	client.passCheck = true; // Password doğruysa, passCheck'i true yapar.
}