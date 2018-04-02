#include "Server.h"
#include "commands.h"
#include <assert.h>
#include <regex>

using std::string;
using std::map;

void userTests() {
    User user;

    assert(user.getUsername() == "");

    user.setUsername("Maria");

    assert(user.getUsername() == "Maria");

    assert(user.userIsOperator() == false);

    user.setOperator(true);

    assert(user.userIsOperator() == true);
}

void channelTests() {
    Channel channel;

    assert(channel.getChannelName() == "");

    channel.setChannelName("#news");

    assert(channel.getChannelName() == "#news");

    assert(channel.containsUser("Maria") == false);

    User Maria = User("Maria");

    channel.addUser(Maria);

    assert(channel.containsUser("Maria") == true);

    User pwnz0r = User("pwnz0r");
    channel.addUser(pwnz0r);
    User john = User("john");
    channel.addUser(john);

    assert(channel.listUsers() == "#news members: Maria john pwnz0r \n");

    assert(channel.containsUser("pwnz0r") == true);

    channel.removeUser("pwnz0r");

    assert(channel.containsUser("pwnz0r") == false);
}

void serverTests() {
    Server serv;

    serv.setPassword("secret");

    assert(serv.checkPassword("password") == false);

    assert(serv.checkPassword("secret") == true);

    serv.addChannel(Channel("#news"));

    assert(serv.containsChannel("#news") == true);

    serv.removeChannel("#news");

    assert(serv.containsChannel("#news") == false);

    serv.addUser(User("Maria"));

    assert(serv.containsUser("Maria") == true);

    serv.removeUser("Maria");

    assert(serv.containsUser("Maria") == false);

    serv.addChannel(Channel("#news"));
    serv.addChannel(Channel("#trivia"));
    serv.addChannel(Channel("#announcements"));

    assert(serv.listChannels() == "* #announcements\n* #news\n* #trivia\n");
}


void USERTests() {
    Server serv;

    assert(USER("Maria", serv) == "Welcome, Maria\n");

    assert(serv.containsUser("Maria") == true);

    assert(USER("pwnZ0r", serv) == "Welcome, pwnZ0r\n");

    assert(serv.containsUser("pwnZ0r") == true);

    assert(USER("ethan riley", serv) == "Invalid username.\n");

    assert(serv.containsUser("ethan riley") == false);
}

void LISTTests() {
    Server serv;

    serv.addChannel(Channel("#news"));
    serv.addChannel(Channel("#trivia"));
    serv.addChannel(Channel("#support"));

    serv.getChannel("#news")->addUser(User("Maria"));
    serv.getChannel("#news")->addUser(User("pwnZ0r"));
    serv.getChannel("#news")->addUser(User("ethan"));

    assert(LIST("", serv) == "* #news\n* #support\n* #trivia\n"); //should document this
    assert(LIST("#news", serv) == "#news members: Maria ethan pwnZ0r \n");
}

void JOINTests() {
    Server serv;

    serv.addChannel(Channel("#news"));
    serv.addUser(User("Maria"));

    assert(JOIN("Maria", "#news", serv) == "Joined channel #news\n");

    assert(JOIN("Maria", "#theory", serv) == "Joined channel #theory\n");
    assert(serv.containsChannel("#theory") == true);
    assert(serv.getChannel("#theory")->containsUser("Maria") == true);

    assert(JOIN("Maria", "new channel", serv) == "Invalid channel name.\n");
    assert(serv.containsChannel("new channel") == false);
}

void PARTTests() {
    Server serv;
    
    serv.addChannel(Channel("#news"));
    serv.addChannel(Channel("#trivia"));
    serv.addChannel(Channel("#support"));

    serv.getChannel("#news")->addUser(User("Maria"));
    serv.getChannel("#trivia")->addUser(User("Maria"));

    assert(PART("Maria", "#news", serv) == "");
    assert(serv.getChannel("#news")->containsUser("Maria") == false);
    assert(serv.getChannel("#trivia")->containsUser("Maria") == true);

    assert(PART("Maria", "#support", serv) == "You are not currently in #support\n");

    serv.getChannel("#news")->addUser(User("Maria"));
    assert(PART("Maria", "", serv) == "");
    assert(serv.getChannel("#news")->containsUser("Maria") == false);
    assert(serv.getChannel("#trivia")->containsUser("Maria") == false);
}

void OPERATORTests() {
    Server serv; 

    serv.setPassword("secret");
    serv.addUser(User("Maria"));
    assert(serv.getUser("Maria")->userIsOperator() == false);

    assert(OPERATOR("Maria", "secret", serv) == "OPERATOR status bestowed.\n");
    assert(serv.getUser("Maria")->userIsOperator() == true);

    assert(OPERATOR("Maria", "password", serv) == "Invalid OPERATOR command.\n");
    assert(serv.getUser("Maria")->userIsOperator() == true); //this is on purpose

    serv.addUser(User("pwnZ0r"));
    assert(OPERATOR("pwnZ0r", "password", serv) == "Invalid OPERATOR command.\n");
    assert(serv.getUser("pwnZ0r")->userIsOperator() == false);
}

void KICKTests() {
    Server serv;

    serv.setPassword("secret");
    serv.addUser(User("Maria"));
    serv.getUser("Maria")->setOperator(true);
    
    serv.addUser(User("pwnZ0r"));

    serv.addChannel(Channel("#news"));

    serv.getChannel("#news")->addUser(User("pwnZ0r"));
    serv.getChannel("#news")->addUser(User("Maria"));

    assert(KICK("Maria", "pwnZ0r", "#news", serv) == "");
    assert(serv.getChannel("#news")->containsUser("pwnZ0r") == false);

    assert(KICK("pwnZ0r", "Maria", "#news", serv) == "You're not op.\n");
    assert(serv.getChannel("#news")->containsUser("Maria") == true);
}

void PRIVMSGTests() {
    Server serv;

    serv.addChannel(Channel("#news"));
    serv.addUser(User("Maria"));
    serv.addUser(User("pwnZ0r"));

    assert(PRIVMSG("Maria", "pwnZ0r", "hey\n", serv) == "");

    assert(PRIVMSG("Maria", "ethan", "hey\n", serv) == "User not found.\n");
    
    assert(PRIVMSG("Maria", "#news", "hey all\n", serv) == "");
    
    assert(PRIVMSG("Maria", "#trivia", "hey all\n", serv) == "Channel not found.\n");
}

void QUITTests() {
    Server serv;
    
    serv.addChannel(Channel("#news"));
    serv.addChannel(Channel("#trivia"));

    serv.getChannel("#news")->addUser(User("Maria"));
    serv.getChannel("#trivia")->addUser(User("Maria"));

    assert(QUIT("Maria", serv) == "");
    assert(serv.getChannel("#news")->containsUser("Maria") == false);
    assert(serv.getChannel("#trivia")->containsUser("Maria") == false);
}

int main(int argc, char** argv) {
    userTests();

    channelTests();

    serverTests();

    USERTests();

    LISTTests();

    JOINTests();

    PARTTests();

    OPERATORTests();

    KICKTests();

    PRIVMSGTests();

    QUITTests();

    return 0;
}
