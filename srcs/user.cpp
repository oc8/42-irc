#include "User.hpp"


//		--> CONSTRUCTORS/DESTRUCTORS <--

User::User() { init_user(); }

User::User(int sd): sd(sd) { init_user(); }

User::User(const User & src) {
	*this = src;
}

User::~User() {}


//		--> GETTERS <--

std::string User::get_username() const { return username; }
std::string User::get_nickname() const {
	if (nickname.empty())
		return "*";
	return nickname;
}
std::string User::get_host() const { return host; }
int User::get_sd() const { return sd; }
bool User::is_logged() const { return connection.connected; }


//		--> SETTERS <--

void User::set_pass(std::string usr_pass, std::string serv_pass) {
	// std::cout << "usr_pass = \"" << usr_pass << "\", serv_pass = \"" << serv_pass << "\"" << std::endl;
	if (usr_pass == serv_pass) {
		connection.pass = true;
		check_if_connected();
	}
	else
		connection.pass = false;
}
void User::set_nickname(std::string new_nickname) {
	nickname = new_nickname;
	// std::cout << "set_nickname user_sd " << sd << " : nickname = " << nickname << std::endl;
	connection.nick = true;
	check_if_connected();
}
void User::set_username(std::string new_username, std::string new_host,
	std::string new_servername, std::string new_realname)
{
	username = new_username;
	connection.user = true;
	host = new_host;
	server_name = new_servername;
	real_name = new_realname;
	check_if_connected();
}


//		--> MEMBER FUNCTIONS <--

void User::init_user() {
	connection.pass = false;
	connection.nick = false;
	connection.user = false;
	connection.connected = false;
	visible = false;
	notif_serv = true;
	wallops = true;
}
void User::check_if_connected() {
	// std::cout << "pass = " << connexion.pass <<
	// ", nick = " << connexion.nick <<
	// ", user = " << connexion.user << std::endl;
	if (connection.pass && connection.nick && connection.user) {
		connection.connected = true;
		char str[] = ":localhost 001 ircserv :Welcome to ircserv! \r\n";
		send(get_sd(), str, strlen(str), 0);
	}
}


//		--> OPERATORS <--

User &User::operator=(const User & src) {
	sd = src.sd;
	nickname = src.nickname;
	username = src.username;
	visible = src.visible;
	notif_serv = src.notif_serv;
	wallops = src.wallops;
	host = src.host;
	server_name = src.server_name;
	real_name = src.real_name;
	connection.pass = src.connection.pass;
	connection.nick = src.connection.nick;
	connection.user = src.connection.user;
	connection.connected = src.connection.connected;
	return *this;
}
