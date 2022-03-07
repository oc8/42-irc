#include <iostream>
#include "channel.hpp"


	//		--> GETTERS/SETTERS <--

std::string Channel::getName() { 
    return name;
}
size_t Channel::getNbUser() { 
    return users.size();
}
size_t Channel::getNbOper() { 
    return operators.size();
}
size_t Channel::getNbTot() { 
    return users.size() + operators.size();
}

// bool Channel::setTopic(User us, std::string newTopic) {
//     if (topic_modif_ope && operators.find(us.getUserName()) != operators.end()) {
//         topic = newTopic;
//         return true;
//     }
//     else if (!topic_modif_ope)  {
//         topic = newTopic;
//         return true;
//     }
//     else
//         return false;
// }

bool Channel::setName (usr_ptr usr, std::string newName) {
    if (is_operator(usr)) {
        name = newName;
        return true;
    }
    return false;
}

	//		--> MEMBER FUCNTIONS <--

void Channel::sendMessage(const char *message) {
    for (user_it it = users.begin(); it != users.end(); it++)
        send((*(it->second)).getSD(), message, strlen(message), 0);
}
bool Channel::add_user(usr_ptr newUser, list &rank) {
    return rank.insert(std::make_pair((*newUser).getUserName(), newUser)).second;
}
bool Channel::del_user(usr_ptr kicked, list &rank) {
    return rank.erase((*kicked).getUserName());
}
bool Channel::is_operator(usr_ptr usr) {
    for (user_it it = operators.begin(); it != operators.end(); it++)
        if (usr == (it->second))
            return true;
    return false;
}
