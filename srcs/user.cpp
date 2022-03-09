#include <iostream>
#include "user.hpp"


//		--> CONSTRUCTORS/DESTRUCTORS <--

User::User() {};

User::User(const User & src) { *this = src; }

User::~User() {};


//		--> GETTERS <--

std::string User::get_username() { return username; }

std::string User::get_nickname() { return nickname; }

int User::get_sd() { return sd; }

bool User::is_logged() { return connexion.connected; }


//		--> SETTERS <--

void User::set_pass(bool is_ok) {
	if (is_ok)
		connexion.pass = true;
	else
		connexion.pass = false;
}
void User::set_nickname(std::string new_nickname) {
	nickname = new_nickname;
	connexion.nick = true;
}
void User::set_username(std::string new_username) {
	username = new_username;
	connexion.user = true;
}


//		--> OPERATORS <--

User User::operator=(const User & src) {
	visible = src.visible;
	return *this;
}