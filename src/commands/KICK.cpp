#include "../../includes/Server.hpp"

void Server::handleKick(const std::string& args, Client& client)
{
    std::vector<std::string> params = splitCommand(args, 1);

    if (params.size() < 2)
    {
        writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "KICK")); // ERR_NEEDMOREPARAMS
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
        writeReply(client.cliFd, ERR_NOSUCHCHANNEL(client.nickName, channelName)); // ERR_NOSUCHCHANNEL
        return;
    }

    if (!channel->isClientInChannel(&client))
    {
        writeReply(client.cliFd, ERR_NOTONCHANNEL(client.nickName, channelName)); // ERR_NOTONCHANNEL
        return;
    }

    if (!client.isOperator)
    {
        writeReply(client.cliFd, ERR_CHANOPRIVSNEEDED(client.nickName, channelName)); // ERR_CHANOPRIVSNEEDED
        return;
    }

    Client *target = findClientByNick(nickToKick);
    if (!target || !channel->isClientInChannel(target))
    {
        writeReply(client.cliFd, ERR_USERNOTINCHANNEL(client.nickName, channelName)); // ERR_USERNOTINCHANNEL
        return;
    }

    std::string kickMsg = ":" + client.nickName + "!" + client.user + "@" + client.ipAddress + " KICK " + channelName + " " + nickToKick + " :" + reason + "\r\n";
    channel->broadcastMessage(kickMsg, NULL, *this);

    channel->removeClient(target); // Operatör devri burada da çalışır
}