#include "../../includes/Server.hpp"

void Server::handleInvite(const std::string& arg, Client& client)
{
    std::vector<std::string> params = splitCommand(arg, 1);

    if (params.size() < 2)
    {
        writeReply(client.cliFd, "Usage: INVITE <nick> <channel>\r\n");
        return;
    }

    std::string nick = params[0];
    std::string channelName = params[1];

    Client* targetClient = findClientByNick(nick);
    Channel* targetChannel = findChannel(channelName);

    if (!targetClient)
    {
        writeReply(client.cliFd, "No such nick/channel\r\n");
        return;
    }

    if (!targetChannel)
    {
        writeReply(client.cliFd, "No such channel\r\n");
        return;
    }

    if (!targetChannel->isClientInChannel(&client))
    {
        writeReply(client.cliFd, "You are not in channel " + channelName + "\r\n");
        return;
    }

    if (targetChannel->isClientInChannel(targetClient))
    {
        writeReply(client.cliFd, "User " + nick + " is already in channel " + channelName + "\r\n");
        return;
    }

    // If we reach this point, we have a valid target client and channel
    targetChannel->addInvitedUser(targetClient);
    writeReply(client.cliFd, "Invited " + nick + " to " + channelName + "\r\n");
    writeReply(targetClient->cliFd, "You have been invited to channel " + channelName + " by " + client.nickName + "\r\n");
}