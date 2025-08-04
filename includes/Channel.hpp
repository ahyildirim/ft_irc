#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"

class Client;

class Channel {
    private:
        std::string _name;
        std::vector<Client *> _clients;
        std::vector<Client *> _operators;
        std::string _topic;

    public:
        Channel(const std::string& name);
        ~Channel();

        const std::string& getName() const;
        const std::string& getTopic() const;

        void setTopic(const std::string& topic);

        void addClient(Client* client);
        void removeClient(Client* client);
        void addOperator(Client* client);
        void broadcastMessage(const std::string& message, Client* sender);
        bool isClientInChannel(Client* client) const;

};

#endif