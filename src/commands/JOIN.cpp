#include "../../includes/Server.hpp"

void Server::handleJoin(const std::string& arg, Client& client)
{
	if (!client.isRegistered)
	{
		writeReply(client.cliFd, ERR_NOTREGISTERED(client.nickName)); // RPL_NOTREGISTERED
		return;
	}

	if (arg.empty())
	{
		writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "JOIN")); // ERR_NEEDMOREPARAMS
		return;
	}

	std::vector<std::string> params = splitCommand(arg, 1);
	std::string channelName = params[0];
	std::string key = (params.size() > 1) ? params[1] : "";

	if (channelName[0] != '#')
	{
		writeReply(client.cliFd, ERR_NOSUCHCHANNEL(client.nickName, channelName)); // ERR_NOSUCHCHANNEL (Geçersiz kanal adı için de kullanılabilir)
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
			writeReply(client.cliFd, ERR_USERONCHANNEL(client.nickName, channelName)); // ERR_USERONCHANNEL
			return;
		}

		if (channel->isInviteOnly() && !isInvited)
		{
			writeReply(client.cliFd, ERR_INVITEONLYCHAN(client.nickName, channelName)); // ERR_INVITEONLYCHAN
			return;
		}

		if (channel->hasKey() && channel->getKey() != key)
		{
			writeReply(client.cliFd, ERR_BADCHANNELKEY(client.nickName, channelName)); // ERR_BADCHANNELKEY
			return;
		}

		if (channel->hasUserLimit() && channel->getClients().size() >= channel->getUserLimit())
		{
			writeReply(client.cliFd, ERR_CHANNELISFULL(client.nickName, channelName)); // ERR_CHANNELISFULL
			return;
		}
	}

	channel->addClient(&client);
	std::string joinMessage = ":" + client.nickName + " JOIN " + channelName + "\r\n";
	channel->broadcastMessage(joinMessage, &client, *this);

	// Topici göster
	if (!channel->getTopic().empty())
		writeReply(client.cliFd, RPL_TOPIC(client.nickName, channelName, channel->getTopic())); // RPL_TOPIC
	else
		writeReply(client.cliFd, RPL_NOTOPIC(client.nickName, channelName)); // RPL_NOTOPIC

	//Kullanıcıları listele
	this->handleNames(channelName, client);
}