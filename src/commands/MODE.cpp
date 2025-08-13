#include "../../includes/Channel.hpp"

void Server::handleMode(const std::string &arg, Client& client)
{
    if (!client.isRegistered)
    {
		writeReply(client.cliFd, "You must register first with NICK and USER commands.\r\n");
		return;
    }

    std::vector<std::string> params = splitBySpace(arg);
    if (params.empty())
    {
        writeReply(client.cliFd, "Usage: MODE <#channel> [<modes> [<mode_parameters>]]\r\n");
        return;
    }

    std::string channelName = params[0];
    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        writeReply(client.cliFd, "403 " + client.nickName + " " + channelName + " :No such channel\r\n");
        return;
    }

    bool isOp = client.isOperator;

    if (params.size() == 1)
    {
        std::string modeString = "+";
        if (channel->isInviteOnly()) modeString += "i";
        if (channel->isTopicRestricted()) modeString += "t";
        if (channel->hasKey()) modeString += "k";
        if (channel->hasUserLimit()) modeString += "l";

        writeReply(client.cliFd, "324 " + client.nickName + " " + channel->getName() + " " + modeString + " " + channel->getKey() + " " + to_string_c98(channel->getUserLimit()) + "\r\n");
        return;
    }

    if (!isOp)
    {
        writeReply(client.cliFd, "482 " + client.nickName + " " + channel->getName() + " :You're not channel operator\r\n");
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
                writeReply(client.cliFd, "MODE " + channel->getName() + " " + (adding ? "+" : "-") + "i\r\n");
                break;
            case 't':
                channel->setTopicRestricted(adding);
                writeReply(client.cliFd, "MODE " + channel->getName() + " " + (adding ? "+" : "-") + "t\r\n");
                break;
            case 'k':
                if (paramIndex >= modeParams.size() && adding)
                {
                    writeReply(client.cliFd, "461 " + client.nickName + " MODE :Not enough parameters\r\n");
                    continue;
                }
                if (adding)
                {
                    channel->setKey(modeParams[paramIndex++]); //? 
                    writeReply(client.cliFd, "MODE " + channel->getName() + " +k " + channel->getKey() + "\r\n");
                }
                else
                {
                    channel->setKey(""); // Remove key
                    writeReply(client.cliFd, "MODE " + channel->getName() + " -k\r\n");
                }
                break;
            case 'o':
                if (paramIndex >= modeParams.size())
                {
                    writeReply(client.cliFd, "461 " + client.nickName + " MODE :Not enough parameters\r\n");
                    continue;
                }

                //Scope for operator mode
                {
                    std::string targetNick = modeParams[paramIndex++];
                    Client* targetClient = findClientByNick(targetNick);

                    if (!targetClient || !channel->isClientInChannel(targetClient))
                    {
                        writeReply(client.cliFd, "401 " + client.nickName + " " + targetNick + " :No such nick/channel\r\n");
                        continue;
                    }

                    if (adding)
                    {
                        channel->addOperator(targetClient);
                        writeReply(client.cliFd, "MODE " + channel->getName() + " +o " + targetNick + "\r\n");
                    }
                    else
                    {
                        channel->removeOperator(targetClient);
                        writeReply(client.cliFd, "MODE " + channel->getName() + " -o " + targetNick + "\r\n");
                    }
                }
                break;
            case 'l':
                if (adding)
                {
                    if (paramIndex >= modeParams.size())
                    {
                        writeReply(client.cliFd, "461 " + client.nickName + " MODE :Not enough parameters\r\n");
                        continue;
                    }
                    try
                    {
                        size_t limit = stoi_c98(modeParams[paramIndex++]);
                        channel->setUserLimit(limit);
                        writeReply(client.cliFd, "MODE " + channel->getName() + " +l " + to_string_c98(limit) + "\r\n");
                    }
                    catch (const std::invalid_argument& ia)
                    {
                        writeReply(client.cliFd, "421 " + client.nickName + " MODE :Invalid argument\r\n");
                        continue;
                    }
                    catch (const std::out_of_range& oor)
                    {
                        writeReply(client.cliFd, "421 " + client.nickName + " MODE :Invalid argument\r\n");
                        continue;
                    }
                }
                else
                {
                    channel->setUserLimit(0);
                    writeReply(client.cliFd, "MODE " + channel->getName() + " -l\r\n");
                }
                break;
            default:
                writeReply(client.cliFd, "472 " + client.nickName + " " + modeChar + " :is unknown mode char to me\r\n");
                break;
        }
    }
}