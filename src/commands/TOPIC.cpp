#include "../../includes/Server.hpp"

void Server::handleTopic(const std::string& arg, Client& client) //[TODO] Make this function compatible with MODE +t
{
	if (!client.isRegistered)
	{
		writeReply(client.cliFd, "You must register first with NICK and USER commands.\r\n");
		return;
	}

	if (arg.empty())
	{
		writeReply(client.cliFd, "Usage: TOPIC <#channel> [:<topic>]\r\n");
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
		writeReply(client.cliFd, "Invalid channel name. Channel names must start with '#'.\r\n");
		return;
	}

	Channel *channel = findChannel(channelName); //olası hata?
	if (!channel)
	{
		writeReply(client.cliFd, "Channel " + channelName + " does not exist.\r\n");
		return;
	}

	if (!channel->isClientInChannel(&client))
	{
		writeReply(client.cliFd, "You are not in channel " + channelName + ".\r\n");
		return;
	}

	if (newTopic.empty())
	{
		// If no new topic is provided, return the current topic
		if (channel->getTopic().empty())
			writeReply(client.cliFd, "331 " + client.nickName + " " + channelName + " :No topic is set\r\n");
		else
			writeReply(client.cliFd, "332 " + client.nickName + " " + channelName + " :" + channel->getTopic() + "\r\n");
	}
	else
	{
		if (!client.isOperator)
		{
			writeReply(client.cliFd, "You must be an operator to set the topic.\r\n");
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