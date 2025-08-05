#include "../includes/Client.hpp"

Client::Client() : buffer(""), isOperator(0), isRegistered(0), passCheck(0), toBeDisconnected(0) {} //passcheck -> 0'a ayarlanacak, test amaçlı 1 yapıldı.

Client::~Client() {}

void Client::setPollWrite(Server &server)
{
    for (size_t i = 1; i < server.pollfds.size(); ++i) 
    {
		if (server.pollfds[i].fd == this->cliFd) 
        {
			server.pollfds[i].events |= POLLOUT;
			break;
		}
	}
}