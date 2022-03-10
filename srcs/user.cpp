#include <iostream>
#include "User.hpp"


//		--> CONSTRUCTORS/DESTRUCTORS <--

User::User() { init_user(); }

User::User(int sd): sd(sd) { init_user(); }

User::User(const User & src) {
	init_user();
	*this = src;
}

User::~User() {}


//		--> GETTERS <--

std::string User::get_username() { return username; }

std::string User::get_nickname() { return nickname; }

int User::get_sd() { return sd; }

bool User::is_logged() { return connexion.connected; }


//		--> SETTERS <--

void User::set_pass(std::string usr_pass, std::string serv_pass) {
	std::cout << "usr_pass = \"" << usr_pass << "\", serv_pass = \"" << serv_pass << "\"" << std::endl;
	if (usr_pass == serv_pass) {
		connexion.pass = true;
		check_if_connected();
	}
	else
		connexion.pass = false;
}
void User::set_nickname(std::string new_nickname) {
	nickname = new_nickname;
	connexion.nick = true;
	check_if_connected();
}
void User::set_username(std::string new_username, std::string new_host,
	std::string new_servername, std::string new_realname)
{
	username = new_username;
	connexion.user = true;
	host = new_host;
	server_name = new_servername;
	real_name = new_realname;
	check_if_connected();
}


//		--> MEMBER FUNCTIONS <--

void User::init_user() {
	connexion.pass = false;
	connexion.nick = false;
	connexion.user = false;
	connexion.connected = false;
	visible = false;
	notif_serv = true;
	wallops = true;
}
void User::check_if_connected() {
	std::cout << "pass = " << connexion.pass << 
	", nick = " << connexion.nick << 
	", user = " << connexion.user << std::endl;
	if (connexion.pass && connexion.nick && connexion.user) {
		connexion.connected = true;
		char str[] = ":localhost 001 ircserv :Welcome to ircserv! \r\n";
		send(get_sd(), str, strlen(str), 0);
	}
}


//		--> OPERATORS <--

User &User::operator=(const User & src) {
	sd = src.sd;
	return *this;
}
