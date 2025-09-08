#include "../../includes/Server.hpp"

void Server::handleJoin(const std::string& arg, Client& client)
{
	if (!client.isRegistered)
	{
		writeReply(client.cliFd, "451 :You have not registered\r\n"); // RPL_NOTREGISTERED
		return;
	}

	if (arg.empty())
	{
		writeReply(client.cliFd, "461 " + client.nickName + " JOIN :Not enough parameters\r\n"); // ERR_NEEDMOREPARAMS
		return;
	}

	std::vector<std::string> params = splitCommand(arg, 1);
	std::string channelName = params[0];
	std::string key = (params.size() > 1) ? params[1] : "";

	if (channelName[0] != '#')
	{
		writeReply(client.cliFd, "403 " + client.nickName + " " + channelName + " :No such channel\r\n"); // ERR_NOSUCHCHANNEL (Geçersiz kanal adı için de kullanılabilir)
		return;
	}

	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	Channel* channel;
	if (it == _channels.end())
	{
		channel = &this->_channels.insert(std::make_pair(channelName, Channel(channelName))).first->second;
		channel->addOperator(&client); // Yeni kanala client operatör olarak eklenir.
		std::cout << GREEN << "Channel " << channelName << " created by " << client.nickName << RESET << std::endl;
	}
	else
	{
		channel = &it->second;
		bool isInvited = channel->isInvited(client.cliFd);
		if (channel->isClientInChannel(&client))
		{
			writeReply(client.cliFd, "443 " + client.nickName + " " + channelName + " :is already on channel\r\n"); // ERR_USERONCHANNEL
			return;
		}

		if (channel->isInviteOnly() && !isInvited)
		{
			writeReply(client.cliFd, "473 " + client.nickName + " " + channelName + " :Cannot join channel (+i)\r\n"); // ERR_INVITEONLYCHAN
			return;
		}

		if (channel->hasKey() && channel->getKey() != key)
		{
			writeReply(client.cliFd, "475 " + client.nickName + " " + channelName + " :Cannot join channel (+k)\r\n"); // ERR_BADCHANNELKEY
			return;
		}

		if (channel->hasUserLimit() && channel->getClients().size() >= channel->getUserLimit())
		{
			writeReply(client.cliFd, "471 " + client.nickName + " " + channelName + " :Cannot join channel (+l)\r\n"); // ERR_CHANNELISFULL
			return;
		}
	}

	channel->addClient(&client);
	std::string joinMessage = ":" + client.nickName + " JOIN " + channelName + "\r\n";
	channel->broadcastMessage(joinMessage, &client, *this);

	// Topici göster
	if (!channel->getTopic().empty())
	{
		std::string topicMessage = "332 " + client.nickName + " " + channelName + " :" + channel->getTopic() + "\r\n"; // RPL_TOPIC
		writeReply(client.cliFd, topicMessage);
	}
	else
	{
		writeReply(client.cliFd, "331 " + client.nickName + " " + channelName + " :No topic is set\r\n"); // RPL_NOTOPIC
	}

	//Kullanıcıları listele
	std::string namesList = "353 " + client.nickName + " = " + channelName + " :";
	for (std::map<int, Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it)
	{
		Client *cl = it->second;
		if (cl)
		{
			if (cl->isOperator)
				namesList += "@"; // Operatörleri işaretle
			namesList += cl->nickName + " ";
		}
	}
	namesList += "\r\n";
	if (!namesList.empty())
		namesList = namesList.substr(0, namesList.length() - 1); // Son boşluğu kaldır
	writeReply(client.cliFd, namesList);
	writeReply(client.cliFd, "366 " + client.nickName + " " + channelName + " :End of /NAMES list.\r\n"); // RPL_ENDOFNAMES

}