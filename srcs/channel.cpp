#include "Channel.hpp"

std::vector<std::string> split(std::string str, std::string delimiter);

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
std::list<User*> Channel::getUsers() {return users;}
std::list<User*> Channel::getOpe() {return operators;}
size_t Channel::getNbUser() { return users.size(); }
size_t Channel::getNbOper() { return operators.size(); }
size_t Channel::getNbTot() { return users.size() + operators.size(); }
bool Channel::getAvail_invit() {return avail_invit;}


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
	exterior_msg = false;
}

void Channel::chan_msg(usr_ref user, std::string message)
{
	if (!is_user(&user) && !is_operator(&user) && !exterior_msg)
		return send_msg(user, ":localhost 404 " + user.get_nickname() + " " + name + " :Cannot send to nick/channel");
	// std::cout << "chan_msg = \"" << message << "\"" << std::endl;
	for (user_ptr_it it = users.begin(); it != users.end(); it++)
		// send((*it)->get_sd(), message.c_str(), strlen(message.c_str()), 0);
		if (*it != &user)
			send_msg(*(*it), message);
	for (user_ptr_it it = operators.begin(); it != operators.end(); it++)
		// send((*it)->get_sd(), message.c_str(), strlen(message.c_str()), 0);
		if (*it != &user)
			send_msg(*(*it), message);
}
void Channel::send_msg(User &user, std::string message) {
    send(user.get_sd(), (message + "\n").c_str(), message.size() + 1, 0);
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
	for (user_ptr_it kick_it = users.begin(); kick_it != users.end(); kick_it++)
		if (*kick_it == kicked) {
			users.erase(kick_it);
			break;
		}
}
void Channel::del_ope(usr_ptr kicked)
{
	for (user_ptr_it kick_it = operators.begin(); kick_it != operators.end(); kick_it++)
		if (*kick_it == kicked) {
			operators.erase(kick_it);
			break;
		}
}
bool Channel::is_user(usr_ptr usr)
{
	for (user_ptr_it it = users.begin(); it != users.end(); it++)
		if (*it == usr)
			return true;
	return false;
}
bool Channel::is_operator(usr_ptr usr)
{
	for (user_ptr_it it = operators.begin(); it != operators.end(); it++)
		if (*it == usr)
			return true;
	return false;
}
bool Channel::is_empty() 
{
	if (getNbUser() == 0 && getNbOper() == 0)
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
	for (user_ptr_it it = users.begin(); it != users.end(); it++)
		ret += (*it)->get_nickname() += " ";
	return ret;
}

std::string Channel::nameOpe() {
	std::string ret;
	for (user_ptr_it it = operators.begin(); it != operators.end(); it++)
		ret += "@" + (*it)->get_nickname() += " ";
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

bool Channel::is_in_channel(User &user){
	for (user_ptr_it user_it = operators.begin(); user_it != operators.end(); ++user_it)
	{
		if ((*user_it)->get_nickname() == user.get_nickname())
			return true;
	}
	for (user_ptr_it user_it = users.begin(); user_it != users.end(); ++user_it)
	{
		if ((*user_it)->get_nickname() == user.get_nickname())
			return true;
	}
	return false;
}

bool Channel::is_banned(User &user){
	for (ban_it it = banned.begin(); it != banned.end(); ++it)
		if (user.get_nickname() == *it)
			return true;
	return false;
}

bool Channel::good_pswd(std::vector<std::string> cmds, size_t i){
	if (cmds.size() >= 3)
	{
		std::vector<std::string> all_pswd = split(cmds[2], ",");
		if (all_pswd.size() >= i && !psw.empty())
			if (psw == all_pswd[i])
				return true;
	}
	else if (psw.empty())
		return true;
	return false;
}


//		--> OPERATORS <--

Channel &Channel::operator=(const Channel & src) {
	name = src.name;
	operators = src.operators;
	users = src.operators;
	max_user = src.max_user;
	psw = src.psw;
	mode = src.mode;
	topic = src.topic;
	topic_modif_ope = src.topic_modif_ope;
	avail_invit = src.avail_invit;
	exterior_msg = src.exterior_msg;
	banned = src.banned;
	return *this;
}
