#include <iostream>
#include "channel.hpp"


	//		--> GETTERS/SETTERS <--

std::string channel::getName() { 
    return name;
}
bool channel::setTopic(user us, std::string newTopic) {
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

void channel::sendMessage(const char *message) {
    for (user_it it = users.begin(); it != users.end(); it++)
        send(it->second.getSD(), message, strlen(message), 0);
}
bool channel::add_user(user newUser) {
    return users.insert(std::make_pair(newUser.getUserName(), newUser)).second;
}
bool channel::del_user(user kicked) {
    return users.erase(kicked.getUserName());
}
