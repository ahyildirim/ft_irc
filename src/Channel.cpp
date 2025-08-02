#include "../includes/Channel.hpp"

Channel::Channel(const std::string& name) : _name(name), _topic("") {}

Channel::~Channel() {}

const std::string& Channel::getName() const 
{
    return this->_name;
}

const std::string& Channel::getTopic() const 
{
    return this->_topic;
}

void Channel::setTopic(const std::string& topic) 
{
    this->_topic = topic;
}

bool Channel::isClientInChannel(Client* client) const 
{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if(_clients[i] == client)
            return true;
    }
    return false;
}

void Channel::addClient(Client* client) 
{
    if (!isClientInChannel(client))
    {
        _clients.push_back(client); // Client'ı kanala ekler.
        std::cout << GREEN << "Client " << client->nickName << " joined channel " << RED << _name << RESET << std::endl;
    }
}

void Channel::removeClient(Client* client)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == client)
        {
            _clients.erase(it);
            std::cout << GREEN << "Client " << client->nickName << " left channel " << RED << _name << RESET << std::endl;
            break;
        }
    }
}

void Channel::broadcastMessage(const std::string& message, Client* sender) 
{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i] != sender) // Gönderen client hariç tüm clientlara mesaj gönderilir.
        {
            _clients[i]->messageBox.push_back(message);
        }
    }
}