#include "../../includes/Server.hpp"

void Server::handleCap(const std::string& arg, Client& client)
{
    if (arg == "LS 302" || arg == "LS")
    {
        writeReply(client.cliFd, "CAP * LS :multi-prefix\r\n");
        client.isCap = true;
    }
    else if (arg.rfind("REQ", 0) == 0)
        writeReply(client.cliFd, "CAP * ACK :multi-prefix\r\n");
    else if (arg == "END")
        return; // Bağlantı tamam, NICK ve USER gelir.
}