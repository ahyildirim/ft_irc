#include "../../includes/Server.hpp"

void Server::handleTopic(const std::string& arg, Client& client)
{
	if (!client.isRegistered)
	{
		writeReply(client.cliFd, ERR_NOTREGISTERED(client.nickName)); // RPL_NOTREGISTERED
		return;
	}

	if (arg.empty())
	{
		writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "TOPIC")); // ERR_NEEDMOREPARAMS
		return;
	}

	std::vector<std::string> tokens = splitCommand(arg, 1);
	std::string channelName = tokens[0];
	std::string newTopic;
	if (tokens.size() > 1)
		newTopic = tokens[1];
	else
		newTopic = "";

	if (channelName.empty() || channelName[0] != '#')
	{
		writeReply(client.cliFd, ERR_NOSUCHCHANNEL(client.nickName, channelName)); // ERR_NOSUCHCHANNEL
		return;
	}

	Channel *channel = findChannel(channelName); //olası hata?
	if (!channel)
	{
		writeReply(client.cliFd, ERR_NOSUCHCHANNEL(client.nickName, channelName)); // ERR_NOSUCHCHANNEL
		return;
	}

	if (!channel->isClientInChannel(&client))
	{
		writeReply(client.cliFd, ERR_NOTONCHANNEL(client.nickName, channelName)); // ERR_NOTONCHANNEL
		return;
	}

	if (newTopic.empty())
	{
		// If no new topic is provided, return the current topic
		if (channel->getTopic().empty())
			writeReply(client.cliFd, RPL_NOTOPIC(client.nickName, channelName));
		else
			writeReply(client.cliFd, RPL_TOPIC(client.nickName, channelName, channel->getTopic()));
	}
	else
	{
		if (!client.isOperator && channel->isTopicRestricted())
		{
			writeReply(client.cliFd, ERR_CHANOPRIVSNEEDED(client.nickName, channelName)); // ERR_CHANOPRIVSNEEDED
			return;
		}
		if (newTopic[0] == ':')
			newTopic = newTopic.substr(1); // Remove the leading ':' if present
		channel->setTopic(newTopic); // Set the new topic for the channel
		writeReply(client.cliFd, "Topic for channel " + channelName + " set to: " + newTopic + "\r\n");
		// Broadcast the new topic to all clients in the channel
		std::string topicMsg = ":" + client.nickName + " TOPIC " + channelName + " :" + newTopic + "\r\n";
		//channel->broadcastMessage(topicMsg, NULL, *this); // [TODO] SEG VERİYOR DÜZELTİLECEK.
	}
}