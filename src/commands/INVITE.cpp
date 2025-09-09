#include "../../includes/Server.hpp"

void Server::handleInvite(const std::string& arg, Client& client)
{
    std::vector<std::string> params = splitCommand(arg, 1);

    if (params.size() < 2)
    {
        writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "INVITE")); // ERR_NEEDMOREPARAMS
        return;
    }

    std::string nick = params[0];
    std::string channelName = params[1];

    Client* targetClient = findClientByNick(nick);
    Channel* targetChannel = findChannel(channelName);

    if (!targetClient || !targetChannel)
    {
        writeReply(client.cliFd, ERR_NOSUCHNICK(client.nickName, nick));
        return;
    }

    if (!targetChannel->isClientInChannel(&client))
    {
        writeReply(client.cliFd, ERR_NOTONCHANNEL(client.nickName, channelName));
        return;
    }

    if (targetChannel->isClientInChannel(targetClient))
    {
        writeReply(client.cliFd, ERR_USERONCHANNEL(nick, channelName));
        return;
    }

    // If we reach this point, we have a valid target client and channel
    targetChannel->addInvitedUser(targetClient);
    writeReply(client.cliFd, RPL_INVITING(client.nickName, channelName, nick));
    writeReply(targetClient->cliFd, RPL_INVITED(client.nickName, channelName));
}