#include <iostream>
#include "channel.hpp"


	//		--> GETTERS/SETTERS <--

std::string Channel::getName() { 
    return name;
}
bool Channel::setTopic(User us, std::string newTopic) {
    if (topic_modif_ope && operators.find(us.getUserName()) != operators.end()) {
        topic = newTopic;
        return true;
    }
    else if (!topic_modif_ope)  {
        topic = newTopic;
        return true;
    }
    else
        return false;
}

	//		--> MEMBER FUCNTIONS <--

void Channel::sendMessage(const char *message) {
    for (user_it it = users.begin(); it != users.end(); it++)
        send(it->second.getSD(), message, strlen(message), 0);
}
bool Channel::add_user(User newUser) {
    return users.insert(std::make_pair(newUser.getUserName(), newUser)).second;
}
bool Channel::del_user(User kicked) {
    return users.erase(kicked.getUserName());
}
