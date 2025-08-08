#include "../../includes/Server.hpp"

void Server::handleKick(const std::string& args, Client& client)
{
    std::vector<std::string> params = splitCommand(args, 1);
    if (params.size() < 2)
    {
        writeReply(client.cliFd, "461 " + client.nickName + " KICK :Not enough parameters\r\n");
        return;
    }

    std::string channelName = params[0];
    std::string targetNick = params[1];

    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        writeReply(client.cliFd, "403 " + client.nickName + " " + channelName + " :No such channel\r\n");
        return;
    }

    if (!channel->isClientInChannel(&client) || !client.isOperator)
    {
        writeReply(client.cliFd, "482 " + client.nickName + " " + channelName + " :You're not channel operator\r\n");
        return;
    }

    Client* target = findClientByNick(targetNick);
    if (!target || !channel->isClientInChannel(target))
    {
        writeReply(client.cliFd, "441 " + client.nickName + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n");
        return;
    }

    std::string kickMsg = ":" + client.nickName + " KICK " + channelName + " " + targetNick + "\r\n";
    channel->broadcastMessage(kickMsg, &client, *this);

    channel->removeClient(target); // Operatör devri burada da çalışır
}