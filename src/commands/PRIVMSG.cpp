#include "../../includes/Server.hpp"

void Server::handlePrivmsg(const std::string& arg, Client& client)
{
    if (!client.isRegistered)
    {
        writeReply(client.cliFd, "You must register first with NICK and USER commands.\r\n");
        return;
    }

    std::vector<std::string> tokens = splitCommand(arg, 1);
    if (tokens.size() < 2)
    {
        writeReply(client.cliFd, "Usage: PRIVMSG <target> :<message>\r\n");
        return;
    }

    std::string target = trim(tokens[0]);
    std::string message = trim(tokens[1]);
    if (message.empty())
    {
        writeReply(client.cliFd, "No text to send.\r\n");
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
            writeReply(client.cliFd, "No such channel: " + target + "\r\n");
            return;
        }
        Channel& channel = it->second;
        if (!channel.isClientInChannel(&client))
        {
            writeReply(client.cliFd, "You are not in channel " + target + ".\r\n");
            return;
        }
        std::string msg = ":" + client.nickName + " PRIVMSG " + target + " :" + message + "\r\n";
        channel.broadcastMessage(msg, &client, *this);
    }
    else
    {
        // Kullanıcıya özel mesaj
        Client* targetClient = findClientByNick(target);
        if (!targetClient)
        {
            writeReply(client.cliFd, "No such nick: " + target + "\r\n");
            return;
        }
        std::string msg = ":" + client.nickName + " PRIVMSG " + target + " :" + message + "\r\n";
        targetClient->messageBox.push_back(msg);
        targetClient->setPollWrite(*this); // Yazma olayını ayarlar. (Issue #1 çözümü)
    }
}