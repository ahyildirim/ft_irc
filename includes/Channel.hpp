#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"

class Client;
class Server;

class Channel {
    private:
        std::string _name;
        std::vector<Client *> _clients;
        std::vector<Client *> _operators;
        std::vector<Client *> _invitedUsers;
        std::string _topic;

        bool _toBeRemoved;
        bool _inviteOnly;
        bool _topicRestricted;
        std::string _key;
        size_t _userLimit;
        
    public:
        //Constructor-Destructor
        Channel(const std::string& name);
        ~Channel();
        
        //Getter & Setter
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getKey() const;
        bool isInviteOnly() const;
        bool isTopicRestricted() const;
        bool isToBeRemoved() const;
        size_t getUserLimit() const;
        std::vector<Client*>& getClients();
        std::vector<Client*>& getInvitedUsers();

        bool hasKey() const;
        bool hasUserLimit() const;

        void setTopic(const std::string& topic);
        void setInviteOnly(bool state);
        void setTopicRestricted(bool state);
        void setKey(const std::string& key);
        void setUserLimit(size_t limit);
        
        //Client Management
        void addClient(Client* client);
        void removeClient(Client* client);
        void addOperator(Client* client);
        void removeOperator(Client* client);
        void broadcastMessage(const std::string& message, Client* sender, Server &server);
        bool isClientInChannel(Client* client) const;
        void addInvitedUser(Client* client);

};

#endif