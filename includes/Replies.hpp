#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

inline std::string RPL_WELCOME(const std::string &nick) {
    return "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n";
}

inline std::string RPL_INVITING(const std::string &nick,
                                const std::string &channel, const std::string &target) {
    return "341 " + nick + " INVITE " + target + " " + channel + "\r\n";
}

inline std::string RPL_INVITED(const std::string &nick,
                                const std::string &channel) {
    return "341 " + nick + " INVITE " + channel + "\r\n";
}

inline std::string ERR_NOSUCHNICK(const std::string &nick, const std::string &target) {
    return "401 " + nick + " " + target + " :No such nick/channel\r\n";
}

inline std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &channel) {
    return "403 " + nick + " " + channel + " :No such channel\r\n";
}

inline std::string ERR_NOTONCHANNEL(const std::string &nick, const std::string &channel) {
    return "442 " + nick + " " + channel + " :You're not on that channel\r\n";
}

inline std::string ERR_CHANOPRIVSNEEDED(const std::string &nick, const std::string &channel) {
    return "482 " + nick + " " + channel + " :You're not channel operator\r\n";
}

inline std::string ERR_USERNOTINCHANNEL(const std::string &nick, const std::string &channel) {
    return "442 " + nick + " " + channel + " :They aren't on that channel\r\n";
}

inline std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &channel) {
    return "443 " + nick + " " + channel + " :is already on channel\r\n";
}

inline std::string ERR_INVITEONLYCHAN(const std::string &nick, const std::string &channel) {
    return "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";
}

inline std::string ERR_BADCHANNELKEY(const std::string &nick, const std::string &channel) {
    return "475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";
}

inline std::string ERR_CHANNELISFULL(const std::string &nick, const std::string &channel) {
    return "471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";
}

inline std::string RPL_NAMREPLY(const std::string &nick,
                                const std::string &channel, const std::string &names) {
    return "353 " + nick + " = " + channel + " :" + names + "\r\n";
}

inline std::string RPL_ENDOFNAMES(const std::string &nick, const std::string &channel) {
    return "366 " + nick + " " + channel + " :End of /NAMES list\r\n";
}

inline std::string ERR_NONICKNAMEGIVEN(const std::string &nick) {
    return "431 " + nick + " :No nickname given\r\n";
}

inline std::string ERR_NICKNAMEINUSE(const std::string &nick, const std::string &newnick) {
    return "433 " + nick + " " + newnick + " :Nickname is already in use\r\n";
}

inline std::string ERR_PASSWDMISMATCH(const std::string &nick) {
    return "464 " + nick + " :Password incorrect\r\n";
}

inline std::string ERR_NOTREGISTERED(const std::string &nick) {
    return "451 " + nick + " :You have not registered\r\n";
}

inline std::string ERR_ALREADYREGISTRED(const std::string &nick) {
    return "462 " + nick + " :You may not reregister\r\n";
}

inline std::string ERR_NOTEXTTOSEND(const std::string &nick) {
    return "412 " + nick + " :No text to send\r\n";
}

inline std::string ERR_UNKNOWNMODE(const char& c, const std::string &channel) {
    return "472 " + std::string(1, c) + " :is unknown mode char to me for" + channel + "\r\n";
}

inline std::string ERR_UNKNOWNCOMMAND(const std::string &nick, const std::string &command) {
    return "421 " + nick + " " + command + " :Unknown command\r\n";
}

inline std::string RPL_CHANNELMODEIS(const std::string &nick, const std::string &channel, const std::string &mode,
                                     const std::string& key, const std::string& limit) {
    return "324 " + nick + " " + channel + " :" + mode + " " + key + " " + limit + "\r\n";
}

inline std::string ERR_NEEDMOREPARAMS(const std::string &nick, const std::string &command) {
    return "461 " + nick + " " + command + " :Not enough parameters\r\n";
}

inline std::string RPL_PRIVMSG(const std::string &prefix, const std::string &target, const std::string &msg) {
    return ":" + prefix + " PRIVMSG " + target + " :" + msg + "\r\n";
}

inline std::string RPL_QUIT(const std::string &prefix, const std::string &msg) {
    return ":" + prefix + " QUIT :" + msg + "\r\n";
}

inline std::string RPL_TOPIC(const std::string &nick,
                             const std::string &channel, const std::string &topic) {
    return "332 " + nick + " " + channel + " :" + topic + "\r\n";
}

inline std::string RPL_NOTOPIC(const std::string &nick, const std::string &channel) {
    return "331 " + nick + " " + channel + " :No topic is set\r\n";
}

#endif // REPLIES_HPP