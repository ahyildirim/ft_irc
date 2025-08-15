#include "../../includes/Server.hpp"

void Server::handlePing(const std::string& arg, Client& client)
{
    std::string response = "PONG :" + (arg.empty() ? client.nickName : arg) + "\r\n";
    writeReply(client.cliFd, response);
}