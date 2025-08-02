#include "../../includes/Server.hpp"

void Server::handleJoin(const std::string& channelName, Client& client)
{
	if (!client.isRegistered)
	{
		writeReply(client.cliFd, "You must register first with NICK and USER commands.\r\n");
		return;
	}

	if (channelName.empty())
		return;
	
	if (channelName[0] != '#')
	{
		writeReply(client.cliFd, "Invalid channel name. Channel names must start with '#'.\r\n");
		return;
	}

	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it == _channels.end())
		it = this->_channels.insert(std::make_pair(channelName, Channel(channelName))).first; // Eğer kanal bulunamazsa, yeni kanal oluşturulur.
	Channel& channel = it->second; // Kanal referansı alınır.

	if (channel.isClientInChannel(&client))
	{
		writeReply(client.cliFd, "You are already in the channel " + channelName + "\r\n");
		return;
	}
	channel.addClient(&client); // Client kanala eklenir.
	std::string joinMessage = ":" + client.nickName + "JOIN" + channelName + "\r\n"; // Join mesajı oluşturulur.
	channel.broadcastMessage(joinMessage, &client); // Kanalda bulunan tüm clientlara mesaj gönderilir.
	writeReply(client.cliFd, "You have joined the channel " + channelName + "\r\n"); // Client'a kanalın başarıyla eklendiği mesajı gönderilir.

	if (!channel.getTopic().empty())
	{
		std::string topicMessage = ":" + std::string("server") + " 332 " + client.nickName + " " + channelName + " :" + channel.getTopic() + "\r\n"; // Kanalın konusu varsa, konu mesajı oluşturulur.
		writeReply(client.cliFd, topicMessage); // Client'a kanalın konusu gönderilir.
	}
}