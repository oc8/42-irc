#include "Bot.hpp"

Bot::Bot(string name): name(name) {}

Bot::~Bot() {}

void Bot::hello(string chan) {
	cout << "Hello " << chan << " !" << endl;
}
