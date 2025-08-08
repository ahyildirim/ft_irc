#include "../includes/Channel.hpp"

Channel::Channel(const std::string& name) : _name(name), _topic(""), toBeRemoved(0) {}

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
        if(_clients[i]->nickName == client->nickName)
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

/* void Channel::removeClient(Client* client)
{
    for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            this->_clients.erase(it);
            std::cout << GREEN << "Client " << client->nickName << " left channel " << RED << _name << RESET << std::endl;
            break;
        }
    }

    for (std::vector<Client*>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            this->_operators.erase(it);
            std::cout << GREEN << "Client " << client->nickName << " removed from operators in channel " << RED << _name << RESET << std::endl;
            break;
        }
    }

    if (this->_clients.empty()) // Eğer kanalda hiç client kalmadıysa, kanalı siler.
    {
        std::cout << RED << "Channel " << _name << " is now empty and will be removed." << RESET << std::endl;
        this->_name.clear(); // Kanal adını temizler.
        this->_topic.clear(); // Kanal konusunu temizler.
        this->_clients.clear(); // Client listesini temizler.
        this->_operators.clear(); // Operatör listesini temizler.
        this->toBeRemoved = true; // Kanalın silinmesi gerektiğini işaretler.
    }
} */

void Channel::removeClient(Client* client)
{
    bool wasOperator = client->isOperator; // Çıkan kişi operatör mü?

    // Client listesinden çıkar
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            _clients.erase(it);
            std::cout << GREEN << "Client " << client->nickName << " left channel " 
                      << RED << _name << RESET << std::endl;
            break;
        }
    }

    // Operatör listesinden çıkar
    for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            _operators.erase(it);
            std::cout << GREEN << "Client " << client->nickName 
                      << " removed from operators in channel " << RED << _name << RESET << std::endl;
            break;
        }
    }

    // Eğer çıkan operatörse ve kanalda başka operatör yoksa, yeni birini ata
    if (wasOperator)
    {
        bool operatorExists = false;
        for (size_t i = 0; i < _clients.size(); ++i)
        {
            if (_clients[i]->isOperator)
            {
                operatorExists = true;
                break;
            }
        }
        if (!operatorExists && !_clients.empty())
        {
            _clients.front()->isOperator = true;
            _operators.push_back(_clients.front());
            std::cout << GREEN << "Client " << _clients.front()->nickName
                      << " is now the operator of channel " << RED << _name << RESET << std::endl;
        }
    }

    // Kanal boş kaldıysa silinmeye işaretle
    if (_clients.empty())
    {
        std::cout << RED << "Channel " << _name << " is now empty and will be removed." << RESET << std::endl;
        _name.clear();
        _topic.clear();
        _clients.clear();
        _operators.clear();
        toBeRemoved = true;
    }
}

void Channel::addOperator(Client* client) 
{
    _operators.push_back(client); // Client'ı kanal operatörü olarak ekler.
    client->isOperator = true; // Client'ın operatör olduğunu işaretler.
    std::cout << GREEN << "Client " << client->nickName << " is now an operator in channel " << RED << _name << RESET << std::endl;
}

void Channel::broadcastMessage(const std::string& message, Client* sender, Server &server) 
{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i] != sender || !sender) // Gönderen client hariç tüm clientlara mesaj gönderilir.
        {
            _clients[i]->messageBox.push_back(message);
            _clients[i]->setPollWrite(server); // Yazma olayını ayarlar. (Issue #1 çözümü)
        }
    }
}
