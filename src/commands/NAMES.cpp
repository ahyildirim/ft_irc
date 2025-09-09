#include "../../includes/Server.hpp"

void Server::handleNames(const std::string& arg, Client &client)
{
    if (!client.isRegistered)
    {
        writeReply(client.cliFd, ERR_NOTREGISTERED(client.nickName)); // RPL_NOTREGISTERED
        return;
    }
    if (arg.empty())
    {
        for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
        {
            Channel &channel = it->second;
            std::string namesList;
            for (std::map<int, Client *>::iterator clientIt = channel.getClients().begin(); clientIt != channel.getClients().end(); ++clientIt)
            {
                if (clientIt->second)
                    namesList += (channel.isOperator(clientIt->first) ? "@" : "") + clientIt->second->nickName + " ";
            }
            if (!namesList.empty())
                namesList = namesList.substr(0, namesList.length() - 1); // Son boşluğu kaldır
            namesList += "\r\n";
            writeReply(client.cliFd, RPL_NAMREPLY(client.nickName, channel.getName(), namesList));
            writeReply(client.cliFd, RPL_ENDOFNAMES(client.nickName, channel.getName())); // RPL_ENDOFNAMES
        }
        return;
    }

    std::stringstream ss(arg);
    std::string channelName;
    while (std::getline(ss, channelName, ','))
    {
        Channel* channel = findChannel(channelName);
        if (!channel)
        {
            writeReply(client.cliFd, RPL_ENDOFNAMES(client.nickName, channelName));
            continue;
        }

        std::string namesList;
        for (std::map<int, Client *>::iterator clientIt = channel->getClients().begin(); clientIt != channel->getClients().end(); ++clientIt)
        {
            if (clientIt->second)
                namesList += (channel->isOperator(clientIt->first) ? "@" : "") + clientIt->second->nickName + " ";
        }
        if (!namesList.empty())
            namesList = namesList.substr(0, namesList.length() - 1); // Son boşluğu kaldır
        writeReply(client.cliFd, RPL_NAMREPLY(client.nickName, channelName, namesList));
        writeReply(client.cliFd, RPL_ENDOFNAMES(client.nickName, channelName)); // RPL_ENDOFNAMES
    }
}