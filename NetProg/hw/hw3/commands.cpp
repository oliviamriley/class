#include "commands.h"

bool validChannelName(std::string channelName) {
    return (std::regex_match(channelName, std::regex("#[a-zA-Z][_0-9a-zA-Z]*")) && channelName.length() <= 20);
}

bool validUsername(std::string username) {
    return (std::regex_match(username, std::regex("[a-zA-Z][_0-9a-zA-Z]*")) && username.length() <= 20);
}

std::string USER(std::string username, Server &serv) {
    if(validUsername(username)) {
        serv.addUser(User(username));
        return "Welcome, " + username + "\n";
    } else {
        return "Invalid username.\n";
    }
}

std::string LIST(std::string channelName, Server &serv) {
    if(serv.containsChannel(channelName)) {
        return serv.getChannel(channelName)->listUsers();
    }
    else {
        return serv.listChannels();
    }
}

std::string JOIN(std::string currentUser, std::string channelName, Server &serv) {
    if(serv.containsChannel(channelName)) {
        serv.getChannel(channelName)->addUser(User(currentUser));
    } else {
        //we have to make the channel
        //validate channelName, create the channel, add our user to channel, add channel to serv
        if(validChannelName(channelName)) {
            Channel newChannel = Channel(channelName);
            newChannel.addUser(User(currentUser));
            serv.addChannel(newChannel);
        } else {
            return "Invalid channel name.\n";
        }
    }
    
    return ("Joined channel " + channelName + "\n");
}

std::string PART(std::string currentUser, std::string channelName, Server &serv) {
    if(channelName.length() > 0) {
        if(serv.containsChannel(channelName)) {
            if(serv.getChannel(channelName)->containsUser(currentUser)) {
                serv.getChannel(channelName)->removeUser(currentUser);
                return "";
            } else {
                return "You are not currently in " + channelName + "\n";
            }
        } else {
            return "Channel not found.\n";
        }
      } else {
        serv.removeFromAllChannels(currentUser);
        return "";
    }
}

//TODO make channels' user dictionaries point to the server's to save memory 

std::string OPERATOR(std::string currentUser, std::string password, Server &serv) {
    if(serv.checkPassword(password)) {
        serv.getUser(currentUser)->setOperator(true);
        return "OPERATOR status bestowed.\n";
    } else {
        return "Invalid OPERATOR command.\n";
    }
}

std::string KICK(std::string currentUser, std::string kickedUser, std::string channelName, Server &serv) {
    if(serv.getUser(currentUser)->userIsOperator()) {
        if(serv.getChannel(channelName)->containsUser(kickedUser)) {
            serv.getChannel(channelName)->kickUser(kickedUser);
            return "";
        }
    } else {
        return "You're not op.\n";
    }
}

std::string PRIVMSG(std::string currentUser, std::string recipient, std::string message, Server &serv) {
    if(message.length() > 512) {
        return "Message too long.\n";
    }

    if(recipient[0] == '#') {
        if(serv.containsChannel(recipient)) {
            serv.getChannel(recipient)->broadcast(message);
            return "";
        } else {
            return "Channel not found.\n";
        }
    } else {
        if(serv.containsUser(recipient)) {
            serv.getUser(recipient)->sendMessage(message);
            return "";
        } else {
            return "User not found.\n";
        }
    }
}

std::string QUIT(std::string currentUser, Server &serv) {
    serv.removeFromAllChannels(currentUser);
    serv.getUser(currentUser)->disconnect();
    return "";
}