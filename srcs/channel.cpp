#include "Channel.hpp"


//		--> CONSTRUCTORS/DESTRUCTORS <--

Channel::Channel() {init_chan();}
Channel::Channel(std::string name) : name(name) {init_chan();}
Channel::Channel(std::string name, usr_ptr ope) : name(name) { 
	init_chan();
	operators.push_back(ope);
}
Channel::Channel(std::string name, std::string psw) : name(name), psw(psw) {init_chan();}
Channel::Channel(const Channel &src) { 
	init_chan();
	*this = src;
}
Channel::~Channel() {}


//		--> GETTERS <--

std::string Channel::getName() { return name; }
std::string Channel::getTopic() { return topic; }
size_t Channel::getNbUser() { return users.size(); }
size_t Channel::getNbOper() { return operators.size(); }
size_t Channel::getNbTot() { return users.size() + operators.size(); }


//		--> SETTERS <--

bool Channel::setName(usr_ptr usr, std::string newName)
{
	if (is_operator(usr))
	{
		name = newName;
		return true;
	}
	return false;
}
bool Channel::setPsw(usr_ptr usr, std::string newPsw)
{
	if (is_operator(usr))
	{
		psw = newPsw;
		return true;
	}
	return false;
}
bool Channel::setMode(usr_ptr usr, std::string newMode)
{
	if (is_operator(usr))
	{
		mode = newMode;
		return true;
	}
	return false;
}
bool Channel::setTopic(usr_ptr usr, std::string newTopic)
{
	if (!topic_modif_ope || (topic_modif_ope && is_operator(usr)))
	{
		topic = newTopic;
		return true;
	}
	return false;
}
bool Channel::setAvailability(usr_ptr usr, bool availability)
{
	if (is_operator(usr))
	{
		avail_invit = availability;
		return true;
	}
	return false;
}


//		--> MEMBER FUCNTIONS <--

void Channel::init_chan() {
	topic_modif_ope = true;
	avail_invit = false;
}

void Channel::sendMessage(const char *message)
{
	for (user_ptr_it it = users.begin(); it != users.end(); it++)
		send((*it)->get_sd(), message, strlen(message), 0);
}
void Channel::add_user(usr_ptr newUser)
{
	users.push_back(newUser);
}
void Channel::add_ope(usr_ptr newUser)
{
	operators.push_back(newUser);
}
void Channel::del_user(usr_ptr kicked)
{
	user_ptr_it kick_it;
	for (kick_it = users.begin(); kick_it != users.end(); kick_it++)
		if (*kick_it == kicked)
			break;
	users.erase(kick_it);
}
void Channel::del_ope(usr_ptr kicked)
{
	user_ptr_it kick_it;
	for (kick_it = operators.begin(); kick_it != operators.end(); kick_it++)
		if (*kick_it == kicked)
			break;
	operators.erase(kick_it);
}
bool Channel::is_operator(usr_ptr usr)
{
	for (user_ptr_it it = operators.begin(); it != operators.end(); it++)
		if (*it == usr)
			return true;
	return false;
}
bool Channel::invitation(usr_ptr inviter, usr_ptr usr)
{
	if (avail_invit && !is_operator(inviter))
		return false;
	add_user(usr);
	return true;
}
std::string Channel::nameUsers() {
	std::string ret;
	for (user_ptr_it it = operators.begin(); it != operators.end(); it++)
		ret += (*it)->get_nickname() += ", ";
	return ret;
}
void Channel::ban_user(std::string nick) {
	ban_it it;
	for (it = banned.begin(); it != banned.end() && nick != *it ; it++)
		;
	if (it == banned.end())
		banned.push_back(nick);
}
void Channel::deban_user(std::string nick) {
	for (ban_it it = banned.begin(); it != banned.end(); it++)
		if (*it == nick) {
			banned.erase(it);
			return;
		}
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
