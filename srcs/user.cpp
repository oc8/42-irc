#include <iostream>
#include "User.hpp"

//		--> CONSTRUCTORS/DESTRUCTORS <--

User::User() { init_user(); }

User::User(int sd): sd(sd) { init_user(); }

User::User(const User & src) {
	*this = src;
}

User::~User() {}

//		--> GETTERS <--

std::string User::get_username() { return username; }

std::string User::get_nickname() { return nickname; }

int User::get_sd() { return sd; }

bool User::is_logged() { return connexion.connected; }

//		--> SETTERS <--



void User::set_pass(bool is_ok) {
	if (is_ok) {
		connexion.pass = true;
		check_if_connected();
	}
	else
		connexion.pass = false;
}
void User::set_nickname(std::string new_nickname)
{
	nickname = new_nickname;
	connexion.nick = true;
	check_if_connected();
}
void User::set_username(std::string new_username)
{
	username = new_username;
	connexion.user = true;
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
	if (connexion.pass && connexion.nick && connexion.user)
		connexion.connected = true;
}

//		--> OPERATORS <--

User &User::operator=(const User & src) {
	visible = src.visible;
	sd = src.sd;
	return *this;
}
