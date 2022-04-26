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
std::list<std::string> Channel::getInvit() {return invit;}
size_t Channel::getNbUser() { return users.size(); }
size_t Channel::getNbOper() { return operators.size(); }
size_t Channel::getNbTot() { return users.size() + operators.size(); }
int Channel::getMaxUser() {return max_user;}
bool Channel::getAvail_invit() {return avail_invit;}
std::list<std::string> Channel::getMode() {return mode;}


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
bool Channel::setTopic(usr_ptr usr, std::string newTopic)
{
	if (topic_editable || (!topic_editable && is_operator(usr)))
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
	topic_editable = true;
	avail_invit = false;
	exterior_msg = false;
	mode.push_back("Cnst");
}

void Channel::chan_msg(usr_ref user, std::string message) {
	(void)user;
	for (user_ptr_it it = users.begin(); it != users.end(); it++)
		send_msg(*(*it), message);
	for (user_ptr_it it = operators.begin(); it != operators.end(); it++)
		send_msg(*(*it), message);
}

void Channel::chan_msg_prv(usr_ref user, std::string message) {
	if (!is_user(&user) && !is_operator(&user) && !exterior_msg && user.get_nickname() != "Botch")
		return send_msg(user, ":localhost 404 " + user.get_nickname() + " " + name + " :Cannot send to nick/channel");
	for (user_ptr_it it = users.begin(); it != users.end(); it++)
		if (*it != &user)
			send_msg(*(*it), message);
	for (user_ptr_it it = operators.begin(); it != operators.end(); it++)
		if (*it != &user)
			send_msg(*(*it), message);
}

void Channel::send_msg(User &user, std::string message) {
    send(user.get_sd(), (message + "\n").c_str(), message.size() + 1, 0);
}

void Channel::add_user(usr_ptr newUser) {
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

void Channel::del_member(usr_ptr member){
	if (is_user(member))
		del_user(member);
	else
		del_ope(member);
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

bool Channel::is_invit(User &usr){
	for (std::list<std::string>::iterator it = invit.begin(); it != invit.end(); ++it)
		if (*it == usr.get_nickname())
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

bool Channel::verif_mode(std::list<std::string> mode, User &user){
	mode.unique();
	for (std::list<std::string>::iterator it = mode.begin(); it != mode.end(); ++it)
	{
		if (*it != "+i" && *it != "+o" && *it != "+p" && *it != "+s" && *it != "+t" && *it != "+n" && *it != "+m"
		&& *it != "+l" && *it != "+b" && *it != "+v" && *it != "+k" && *it != "-i" && *it != "-o" && *it != "-p"
		&& *it != "-s" && *it != "-t" && *it != "-n" && *it != "-m" && *it != "-l" && *it != "-b" && *it != "-v" && *it != "-k")
		{
			send_msg(user, ":localhost 472 " + user.get_nickname() + " " + (*it)[1] + " :is an unknow mode char to me");
			return false;
		}
	}
	return true;
}

void Channel::invite_mode(char sign, std::list<std::string> *ret){
	if (avail_invit == false && sign == '+')
	{
		add_ret_mode(ret, std::string() + sign + 'i', "");
		avail_invit = true;
		this->mode.front().push_back('i');
	}
	else if (avail_invit == true && sign == '-')
	{
		add_ret_mode(ret, std::string() + sign + 'i', "");
		this->mode.front().erase(remove(mode.front().begin(), mode.front().end(), 'i'), mode.front().end());
		avail_invit = false;
	}
}

void Channel::op_mode(User &user, char sign, std::vector<std::string> cmds, std::list<std::string> *ret){
	if (cmds.size() < 4)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " MODE :Not enough parameters"));
	for (user_ptr_it it = operators.begin(); it != operators.end(); ++it)
	{
		if ((*it)->get_nickname() == cmds[3] && sign == '+')
		{
			add_ret_mode(ret, std::string() + sign + 'o', cmds[3]);
			cmds.erase(cmds.begin() + 3);
			return;
		}
		else if ((*it)->get_nickname() == cmds[3] && sign == '-')
		{
			users.push_back(*it);
			it = operators.erase(it);
			add_ret_mode(ret, std::string() + sign + 'o', cmds[3]);
			cmds.erase(cmds.begin() + 3);
			return;
		}
	}
	if (sign == '+')
	{
		for (user_ptr_it it = users.begin(); it != users.end(); ++it)
		{
			operators.push_back(*it);
			it = users.erase(it);
			add_ret_mode(ret, std::string() + sign + 'o', cmds[3]);
			cmds.erase(cmds.begin() + 3);
			return;
		}
	}
	send_msg(user, ":localhost 401 " + user.get_nickname() + " :No such nick/channel");
	cmds.erase(cmds.begin() + 3);
}

void Channel::limit_mode(User &user, char sign, std::vector<std::string> cmds, std::list<std::string> *ret){
	if (sign == '+')
	{
		if (cmds.size() < 4)
			return (send_msg(user, ":localhost 461 " + user.get_nickname() + " MODE :Not enough parameters"));
		std::istringstream(cmds[3]) >> max_user;
		if (max_user <= 0)
			return;
		if (mode.front().find("l") == std::string::npos)
		{
			mode.push_back(std::to_string(max_user));
			mode.front().push_back('l');
		}
		else
		{
			std::list<std::string>::iterator it = std::find(mode.begin(), mode.end(), std::to_string(max_user));
			*it = std::to_string(max_user);
		}
		add_ret_mode(ret, std::string() + sign + "l", std::to_string(max_user));
		cmds.erase(cmds.begin() + 3);
 	}
	else
	{
		this->mode.front().erase(remove(mode.front().begin(), mode.front().end(), 'l'), mode.front().end());
		std::list<std::string>::iterator it = std::find(mode.begin(), mode.end(), std::to_string(max_user));
		mode.erase(it);
		max_user = 0;
		add_ret_mode(ret, std::string() + sign + "l", "");
	}
}

void Channel::psw_mode(char sign, std::vector<std::string> cmds, std::list<std::string> *ret){
	if (cmds.size() < 3)
		return;
	if (sign == '-')
	{
		if (psw.empty())
			return;
		this->mode.front().erase(remove(mode.front().begin(), mode.front().end(), 'k'), mode.front().end());
		std::list<std::string>::iterator it = std::find(mode.begin(), mode.end(), psw);
		mode.erase(it);
		psw.clear();
		add_ret_mode(ret, std::string() + sign + "k", "*");
		cmds.erase(cmds.begin() + 3);
	}
	else
	{
		psw = cmds[3];
		if (mode.front().find("k") == std::string::npos)
		{
			mode.push_back(psw);
			mode.front().push_back('k');
		}
		else
		{
			std::list<std::string>::iterator it = std::find(mode.begin(), mode.end(), psw);
			*it = psw;
		}
		add_ret_mode(ret, std::string() + sign + "k", "*");
		cmds.erase(cmds.begin() + 3);
	}
}

void Channel::topic_mode(char sign, std::list<std::string> *ret){
	if ((sign == '+' && !topic_editable) || (sign == '-' && topic_editable))
	{
		topic_editable = !topic_editable;
		add_ret_mode(ret, std::string() + sign + "t", "");
		if (sign == '+' && !mode.front().find("t"))
			mode.front().push_back('t');
		else if (sign == '-')
			this->mode.front().erase(remove(mode.front().begin(), mode.front().end(), 't'), mode.front().end());
	}
}

void Channel::ext_msg_mode(char sign, std::list<std::string> *ret){
	if ((sign == '+' && exterior_msg) || (sign == '-' && !exterior_msg))
	{
		exterior_msg = !exterior_msg;
		add_ret_mode(ret, std::string() + sign + "n", "");
		if (sign == '+' && !mode.front().find("n"))
			mode.front().push_back('n');
		else if (sign == '-')
			this->mode.front().erase(remove(mode.front().begin(), mode.front().end(), 'n'), mode.front().end());
	}
}

void Channel::exec_mode(User &user, std::list<std::string> mode, std::vector<std::string> cmds, std::list<std::string> *ret){
	for (std::list<std::string>::iterator it = mode.begin(); it != mode.end(); ++it)
	{
		if (*it == "+i" || *it == "-i")
			invite_mode((*it)[0], ret);
		else if (*it == "+o" || *it == "-o")
			op_mode(user, (*it)[0], cmds, ret);
		else if (*it == "+l" || *it == "-l")
			limit_mode(user, (*it)[0], cmds, ret);
		else if (*it == "+k" || *it == "-k")
			psw_mode((*it)[0], cmds, ret);
		else if (*it == "+t" || *it == "-t")
			topic_mode((*it)[0], ret);
		else if (*it == "+n" || *it == "-n")
			ext_msg_mode((*it)[0], ret);
	}
}

void Channel::add_ret_mode(std::list<std::string> *ret, std::string mode, std::string target){
	if (ret->empty())
		ret->push_back(mode);
	else
	{
		if (ret->front().empty() || ret->front()[ret->front().size() - 2] != mode[0])
			ret->front().push_back(mode[0]);
		ret->front().push_back(mode[1]);
	}
	if (!target.empty())
		ret->push_back(target);
}

std::string Channel::display_ret(std::list<std::string> ret){
	std::string display;

	for (std::list<std::string>::iterator it = ret.begin(); it != ret.end(); ++it)
		display += *it + " ";
	return display;
}

std::string Channel::display_mode(){
	std::string ret;

	if (!mode.empty())
	{
		ret += "+";
		for (std::list<std::string>::iterator it = mode.begin(); it != mode.end(); ++it)
			ret += *it + " ";
	}
	return ret;
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
	topic_editable = src.topic_editable;
	avail_invit = src.avail_invit;
	exterior_msg = src.exterior_msg;
	banned = src.banned;
	return *this;
}
