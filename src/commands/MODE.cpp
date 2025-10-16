#include "../../includes/Channel.hpp"

void Server::handleMode(const std::string &arg, Client& client)
{
    if (!client.isRegistered)
    {
		writeReply(client.cliFd, ERR_NOTREGISTERED(client.nickName)); // RPL_NOTREGISTERED
		return;
    }

    std::vector<std::string> params = splitBySpace(arg);
    if (params.empty())
    {
        writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "MODE")); // ERR_NEEDMOREPARAMS
        return;
    }

    std::string channelName = params[0];
    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        writeReply(client.cliFd, ERR_NOSUCHCHANNEL(client.nickName, channelName)); // ERR_NOSUCHCHANNEL
        return;
    }

    bool isOp = client.isOperator;

    if (params.size() == 1)
    {
        std::string modeString = "+";
        if (channel->isInviteOnly()) 
            modeString += "i";
        if (channel->isTopicRestricted()) 
            modeString += "t";
        if (channel->hasKey()) 
            modeString += "k";
        if (channel->hasUserLimit()) 
            modeString += "l";

        writeReply(client.cliFd, RPL_CHANNELMODEIS(client.nickName, channelName, modeString,
                                              channel->hasKey() ? channel->getKey() : "",
                                              channel->hasUserLimit() ? to_string_c98(channel->getUserLimit()) : ""));
        return;
    }

    if (!isOp)
    {
        writeReply(client.cliFd, ERR_CHANOPRIVSNEEDED(client.nickName, channelName)); // ERR_CHANOPRIVSNEEDED
        return;
    }

    std::string modes = params[1];
    std::vector<std::string> modeParams;
    for (size_t i = 2; i < params.size(); ++i)
        modeParams.push_back(params[i]);
    
    bool adding = true;
    size_t paramIndex = 0;

    for (size_t i = 0; i < modes.size(); ++i)
    {
        char modeChar = modes[i];
        if (modeChar == '+')
        {
            adding = true;
            continue;
        }
        else if (modeChar == '-')
        {
            adding = false;
            continue;
        }

        switch(modeChar)
        {
            case 'i':
                channel->setInviteOnly(adding);
                break;
            case 't':
                channel->setTopicRestricted(adding);
                break;
            case 'k':
                if (paramIndex >= modeParams.size() && adding)
                {
                    writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "MODE")); // ERR_NEEDMOREPARAMS
                    continue;
                }
                if (adding)
                    channel->setKey(modeParams[paramIndex++]);
                else
                    channel->setKey("");
                break;
            case 'o':
                if (paramIndex >= modeParams.size())
                {
                    writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "MODE")); // ERR_NEEDMOREPARAMS
                    continue;
                }

                //Scope for operator mode
                {
                    std::string targetNick = modeParams[paramIndex++];
                    Client* targetClient = findClientByNick(targetNick);

                    if (!targetClient || !channel->isClientInChannel(targetClient))
                    {
                        writeReply(client.cliFd, ERR_NOSUCHNICK(client.nickName, targetNick)); // ERR_NOSUCHNICK
                        continue;
                    }

                    if (adding)
                        channel->addOperator(targetClient);
                    else
                        channel->removeOperator(targetClient);
                }
                break;
            case 'l':
                if (adding)
                {
                    if (paramIndex >= modeParams.size())
                    {
                        writeReply(client.cliFd, ERR_NEEDMOREPARAMS(client.nickName, "MODE")); // ERR_NEEDMOREPARAMS
                        continue;
                    }
                    try
                    {
                        size_t limit = stoi_c98(modeParams[paramIndex++]);
                        channel->setUserLimit(limit);
                    }
                    catch (const std::invalid_argument& ia)
                    {
                        writeReply(client.cliFd, ERR_UNKNOWNCOMMAND(client.nickName, "MODE")); // ERR_UNKNOWNCOMMAND
                        continue;
                    }
                    catch (const std::out_of_range& oor)
                    {
                        writeReply(client.cliFd, ERR_UNKNOWNCOMMAND(client.nickName, "MODE")); // ERR_UNKNOWNCOMMAND
                        continue;
                    }
                }
                else
                    channel->setUserLimit(0);
                break;
            default:
                writeReply(client.cliFd, ERR_UNKNOWNMODE(modeChar, channelName)); // ERR_UNKNOWNMODE
                break;
        }
    }
}