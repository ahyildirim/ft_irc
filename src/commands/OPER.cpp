#include "../../includes/Server.hpp"

// OPER <nickname>
void Server::handleOper(const std::string &args, Client &client)
{
    std::vector<std::string> params = splitCommand(args, 1);

    // Parametre kontrolü
    if (params.size() < 1 || params[0].empty())
    {
        writeReply(client.cliFd, "461 " + client.nickName + " OPER :Not enough parameters\r\n");
        return;
    }

    std::string targetNick = params[0];

    // Komutu gönderenin operatör yetkisi var mı?
    if (!client.isOperator)
    {
        writeReply(client.cliFd, "481 " + client.nickName + " :Permission Denied - You're not an IRC operator\r\n");
        return;
    }

    // Hedef kullanıcıyı bul
    Client *target = findClientByNick(targetNick);
    if (!target)
    {
        writeReply(client.cliFd, "401 " + client.nickName + " " + targetNick + " :No such nick\r\n");
        return;
    }

    // Zaten operatör mü?
    if (target->isOperator)
    {
        writeReply(client.cliFd, "481 " + client.nickName + " " + targetNick + " :User is already an operator\r\n");
        return;
    }

    // Operatör yap
    target->isOperator = true;
    writeReply(client.cliFd, ":" + client.nickName + " MODE +o " + targetNick + "\r\n");
    writeReply(target->cliFd, ":" + client.nickName + " has made you an operator\r\n");

    std::cout << GREEN << "Client " << targetNick << " is now an IRC operator." << RESET << std::endl;
}

