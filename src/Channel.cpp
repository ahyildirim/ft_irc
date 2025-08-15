#include "../includes/Channel.hpp"

//Constructor-Destructor
Channel::Channel(const std::string& name) : _name(name), _topic(""), _toBeRemoved(0), _inviteOnly(0), _topicRestricted(1), _userLimit(0) {}

Channel::~Channel() {}


const std::string& Channel::getName() const 
{
    return this->_name;
}

const std::string& Channel::getTopic() const 
{
    return this->_topic;
}

const std::string& Channel::getKey() const
{
    return this->_key;
}

bool Channel::isToBeRemoved() const
{
    return this->_toBeRemoved;
}

bool Channel::isInviteOnly() const
{
    return this->_inviteOnly;
}

bool Channel::isTopicRestricted() const
{
    return this->_topicRestricted;
}

size_t Channel::getUserLimit() const
{
    return this->_userLimit;
}

std::vector<Client*>& Channel::getInvitedUsers()
{
    return this->_invitedUsers;
}

std::vector<Client*>& Channel::getClients()
{
    return this->_clients;
}

bool Channel::hasKey() const
{
    return !this->_key.empty();
}

bool Channel::hasUserLimit() const
{
    return this->_userLimit > 0;
}



void Channel::setTopic(const std::string& topic) 
{
    this->_topic = topic;
}

void Channel::setInviteOnly(bool state)
{
    this->_inviteOnly = state;
}

void Channel::setTopicRestricted(bool state)
{
    this->_topicRestricted = state;
}

void Channel::setKey(const std::string& key)
{
    this->_key = key;
}

void Channel::setUserLimit(size_t limit)
{
    this->_userLimit = limit;
}


//Client Management
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

void Channel::removeClient(Client* client)
{
    bool wasOperator = client->isOperator; // Çıkan kişi operatör mü?

    // Client listesinden çıkar
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            _clients.erase(it);
            client->isOperator = false;
            std::cout << GREEN << "Client " << client->nickName << " left channel " << RED << _name << RESET << std::endl;
            break;
        }
    }

    // Operatör listesinden çıkar (eğer listedeyse)
    for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            _operators.erase(it);
            std::cout << GREEN << "Client " << client->nickName << " removed from operators in channel " << RED << _name << RESET << std::endl;
            break;
        }
    }

    for (std::vector<Client*>::iterator it = _invitedUsers.begin(); it != _invitedUsers.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            _invitedUsers.erase(it);
            std::cout << GREEN << "Client " << client->nickName << " removed from invited users in channel " << RED << _name << RESET << std::endl;
            break;
        }
    }

    // Eğer çıkan operatörse ve kanalda hala client varsa, yeni bir operatör ata
    if (wasOperator && !_clients.empty()) // <-- !_clients.empty() kontrolünü ekleyin
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
        // Kanala yeni bir operatör atanmamışsa, ilk client'ı operatör yap
        if (!operatorExists) // <-- !operatorExists kontrolünü ekleyin
        {
            _clients.front()->isOperator = true;
            _operators.push_back(_clients.front());
            std::cout << GREEN << "Client " << _clients.front()->nickName << " is now the operator of channel " << RED << _name << RESET << std::endl;
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
        _invitedUsers.clear();
        _toBeRemoved = true;
    }
}

void Channel::addOperator(Client* client) 
{
    _operators.push_back(client); // Client'ı kanal operatörü olarak ekler.
    client->isOperator = true; // Client'ın operatör olduğunu işaretler.
    std::cout << GREEN << "Client " << client->nickName << " is now an operator in channel " << RED << _name << RESET << std::endl;
}

void Channel::removeOperator(Client* client) 
{
    for (std::vector<Client*>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
    {
        if ((*it)->nickName == client->nickName)
        {
            this->_operators.erase(it);
            client->isOperator = false; // Client'ın operatörlük yetkisini sıfırlar.
            std::cout << GREEN << "Client " << client->nickName << " removed from operators in channel " << RED << _name << RESET << std::endl;
            break;
        }
    }
}

void Channel::addInvitedUser(Client* client) 
{
    // Zaten davet edilmiş mi kontrol et
    for (size_t i = 0; i < _invitedUsers.size(); ++i)
    {
        if (_invitedUsers[i] == client) // Pointer karşılaştırması
            return; // Zaten listede, tekrar ekleme
    }

    // Yoksa ekle
    _invitedUsers.push_back(client);
}

void Channel::broadcastMessage(const std::string& message, Client* sender, Server &server) //ne oe bi fonksiyon bu ya
{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if ((!sender || _clients[i] != sender) && !_clients[i]->toBeDisconnected) // Gönderen client hariç tüm clientlara mesaj gönderilir.
        {
            _clients[i]->messageBox.push_back(message);
            _clients[i]->setPollWrite(server); // Yazma olayını ayarlar. (Issue #1 çözümü)
        }
    }
}
