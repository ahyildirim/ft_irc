#include "../../includes/Server.hpp"

void Server::handlePrivmsg(const std::string& arg, Client& client)
{
    if (!client.isRegistered)
    {
        writeReply(client.cliFd, ERR_NOTREGISTERED(client.nickName)); // RPL_NOTREGISTERED
        return;
    }

    std::vector<std::string> tokens = splitCommand(arg, 1);
    if (tokens.size() < 2)
    {
        writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "PRIVMSG")); // ERR_NEEDMOREPARAMS
        return;
    }

    std::string target = trim(tokens[0]);
    std::string message = trim(tokens[1]);
    if (message.empty())
    {
        writeReply(client.cliFd, ERR_NOTEXTTOSEND(client.nickName)); // ERR_NOTEXTTOSEND
        return;
    }
    if (message[0] == ':')
        message = message.substr(1);

    // Channel mesajı mı, kullanıcı mesajı mı?
    if (!target.empty() && target[0] == '#')
    {   
        std::map<std::string, Channel>::iterator it = _channels.find(target);
        if (it == _channels.end())
        {
            writeReply(client.cliFd, ERR_NOSUCHCHANNEL(client.nickName, target)); // ERR_NOSUCHCHANNEL
            return;
        }
        Channel& channel = it->second;
        if (!channel.isClientInChannel(&client))
        {
            writeReply(client.cliFd, ERR_NOTONCHANNEL(client.nickName, target)); // ERR_NOTONCHANNEL
            return;
        }
        channel.broadcastMessage(RPL_PRIVMSG(client.nickName, target, message), &client, *this);
    }
    else
    {
        // Kullanıcıya özel mesaj
        Client* targetClient = findClientByNick(target);
        if (!targetClient)
        {
            writeReply(client.cliFd, ERR_NOSUCHNICK(client.nickName, target)); // ERR_NOSUCHNICK
            return;
        }
        targetClient->messageBox.push_back(RPL_PRIVMSG(client.nickName, target, message));
        targetClient->setPollWrite(*this); // Yazma olayını ayarlar. (Issue #1 çözümü)
    }
}