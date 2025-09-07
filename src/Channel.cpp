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

std::map<int, Client*>& Channel::getInvitedUsers()
{
    return this->_invitedUsers;
}

std::map<int, Client*>& Channel::getClients()
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
    if (!client)
        return false;
    return _clients.find(client->cliFd) != _clients.end();
}

void Channel::addClient(Client* client) 
{
    if (!client)
        return;
    if (!isClientInChannel(client))
    {
        _clients.insert(std::make_pair(client->cliFd, client));
        std::cout << GREEN << "Client " << client->nickName << " joined channel " << RED << _name << RESET << std::endl;
    }
}

void Channel::removeClient(Client* client)
{
    if (!client)
        return;
    bool wasOperator = isOperator(client->cliFd);
    _clients.erase(client->cliFd);
    _operators.erase(client->cliFd);
    _invitedUsers.erase(client->cliFd);

    std::cout << GREEN << "Client " << client->nickName << " left channel " << RED << _name << RESET << std::endl;

    if (wasOperator && !_clients.empty() && _operators.empty())
    {
        Client* newOp = _clients.begin()->second;
        addOperator(newOp);
        std::cout << GREEN << "Client " << newOp->nickName << " is now an operator in channel " << RED << _name << RESET << std::endl;
    }

    if (_clients.empty())
    {
        _toBeRemoved = true;
        std::cout << RED << "Channel " << _name << " is now empty and will be removed." << RESET << std::endl;
    }
}

bool Channel::isOperator(int clientFd) const
{
    return _operators.find(clientFd) != _operators.end();
}

void Channel::addOperator(Client* client) 
{
    if (!client)
        return;
    if (!isOperator(client->cliFd))
    {
        _operators.insert(std::make_pair(client->cliFd, client));
        client->isOperator = true;
        std::cout << GREEN << "Client " << client->nickName << " is now an operator in channel " << RED << _name << RESET << std::endl;
    }
}

void Channel::removeOperator(Client* client) 
{
    if (!client)
        return;
    _operators.erase(client->cliFd);
    client->isOperator = false;
    std::cout << GREEN << "Client " << client->nickName << " removed from operators in channel " << RED << _name << RESET << std::endl;
}

bool Channel::isInvited(int clientFd) const
{
    return _invitedUsers.find(clientFd) != _invitedUsers.end();
}

void Channel::addInvitedUser(Client* client) 
{
    if (!client)
        return;
    if (!isInvited(client->cliFd))
        _invitedUsers.insert(std::make_pair(client->cliFd, client));
}

void Channel::broadcastMessage(const std::string& message, Client* sender, Server &server)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        Client* client = it->second;
        if (!client || (sender && client->cliFd == sender->cliFd))
            continue;
        client->messageBox.push_back(message);
        client->setPollWrite(server); // Yazma olayını ayarlar. (Issue #1 çözümü)
    }
}
