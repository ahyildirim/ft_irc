#include "../../includes/Server.hpp"

void Server::handleKick(const std::string& args, Client& client)
{
    std::vector<std::string> params = splitCommand(args, 1);

    if (params.size() < 2)
    {
        writeReply(client.cliFd, "Usage: KICK <#channel> <nick> [<reason>]\r\n");
        return;
    }

    std::string channelName = params[0];
    std::string nickToKick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : "Kicked by operator";
    if (reason[0] == ':')
        reason = reason.substr(1);
    
    Channel *channel = findChannel(channelName);
    if (!channel)
    {
        writeReply(client.cliFd, "403 " + client.nickName + " " + channelName + " :No such channel\r\n");
        return;
    }

    if (!channel->isClientInChannel(&client))
    {
        writeReply(client.cliFd, "442 " + client.nickName + " " + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (!client.isOperator)
    {
        writeReply(client.cliFd, "482 " + client.nickName + " " + channelName + " :You're not channel operator\r\n");
        return;
    }

    Client *target = findClientByNick(nickToKick);
    if (!target || !channel->isClientInChannel(target))
    {
        writeReply(client.cliFd, "441 " + client.nickName + " " + nickToKick + " " + channelName + " :They aren't on that channel\r\n");
        return;
    }

    std::string kickMsg = ":" + client.nickName + "!" + client.user + "@" + client.ipAddress + " KICK " + channelName + " " + nickToKick + " :" + reason + "\r\n";
    channel->broadcastMessage(kickMsg, NULL, *this);

    channel->removeClient(target); // Operatör devri burada da çalışır
}