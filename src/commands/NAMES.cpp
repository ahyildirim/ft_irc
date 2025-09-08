#include "../../includes/Server.hpp"

void Server::handleNames(const std::string& arg, Client &client)
{
    if (!client.isRegistered)
    {
        writeReply(client.cliFd, "451 :You have not registered\r\n"); // RPL_NOTREGISTERED
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
            writeReply(client.cliFd, "353 " + client.nickName + " = " + channel.getName() + " :" + namesList + "\r\n");
            writeReply(client.cliFd, "366 " + client.nickName + " " + channel.getName() + " :End of /NAMES list.\r\n");
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
            writeReply(client.cliFd, "366 " + client.nickName + " " + channelName + " :End of /NAMES list.\r\n");
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
        writeReply(client.cliFd, "353 " + client.nickName + " = " + channel->getName() + " :" + namesList + "\r\n");
        writeReply(client.cliFd, "366 " + client.nickName + " " + channel->getName() + " :End of /NAMES list.\r\n");
    }
}