#include "Bot.hpp"

Bot::Bot(std::string name) {
	User::set_nickname(name);
	User::set_username(name, "0", "*", ":botch");
}

Bot::~Bot() {}

void Bot::hello(Channel &chan) {
	chan.chan_msg(*this, "Le bot " + get_nickname() + " est prêt !");
}

void Bot::welcome(Channel &chan, User &user) {
	// std::string chan = "";
	// for (int i = 0; i < 6; i++) {
	// 	chan += (char)(rand() % 26 + 'a');
	// }
	// return chan;
	chan.chan_msg(*this, ":" + this->get_nickname() + "!~" + this->get_username() + "@" +
		this->get_host() + " Salut " + user.get_nickname() + ", bienvenue sur le canal " + chan.getName() + " !");
}
