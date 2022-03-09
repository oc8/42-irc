#include <iostream>
#include "channel.hpp"

//		--> CONSTRUCTORS/DESTRUCTORS <--

Channel::Channel() {}
Channel::Channel(std::string name) : name(name) {}
Channel::Channel(std::string name, std::string psw) : name(name), psw(psw) {}
Channel::Channel(const Channel& src) { *this = src; }
Channel::~Channel() {}


//		--> GETTERS <--

std::string Channel::getName() { return name; }
size_t Channel::getNbUser() { return users.size(); }
size_t Channel::getNbOper() { return operators.size(); }
size_t Channel::getNbTot() { return users.size() + operators.size(); }


//		--> SETTERS <--

bool Channel::setName (usr_ptr usr, std::string newName) {
    if (is_operator(usr)) {
        name = newName;
        return true;
    }
    return false;
}
bool Channel::setPsw (usr_ptr usr, std::string newPsw) {
    if (is_operator(usr)) {
        psw = newPsw;
        return true;
    }
    return false;
}
bool Channel::setMode (usr_ptr usr, std::string newMode) {
    if (is_operator(usr)) {
        mode = newMode;
        return true;
    }
    return false;
}
bool Channel::setTopic (usr_ptr usr, std::string newTopic) {
    if (!topic_modif_ope || (topic_modif_ope && is_operator(usr))) {
        topic = newTopic;
        return true;
    }
    return false;
}
bool Channel::setAvailability (usr_ptr usr, bool availability) {
    if (is_operator(usr)) {
        avail_invit = availability;
        return true;
    }
    return false;
}


//		--> MEMBER FUCNTIONS <--

void Channel::sendMessage(const char *message) {
    for (user_it it = users.begin(); it != users.end(); it++)
        send((*(it->second)).get_sd(), message, strlen(message), 0);
}
bool Channel::add_user(usr_ptr newUser, list &rank) {
    return rank.insert(std::make_pair((*newUser).get_nickname(), newUser)).second;
}
bool Channel::del_user(usr_ptr kicked, list &rank) {
    return rank.erase((*kicked).get_nickname());
}
bool Channel::is_operator(usr_ptr usr) {
    for (user_it it = operators.begin(); it != operators.end(); it++)
        if (usr == (it->second))
            return true;
    return false;
}
bool Channel::invitation(usr_ptr inviter, usr_ptr usr) {
    if (avail_invit && !is_operator(inviter))
        return false;
    add_user(usr, users);
    return true;
}


//		--> OPERATORS <--

Channel &Channel::operator=(const Channel & src) {
	psw = src.psw;
	mode = src.mode;
	topic_modif_ope = src.topic_modif_ope;
	topic = src.topic;
	avail_invit = src.avail_invit;
	return *this;
}
