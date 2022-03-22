#include "Bot.hpp"

Bot::Bot(std::string name) : name(name), sock(0) {}

Bot::~Bot() {}

void Bot::socket()
{
	if ((sock = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Socket creation error" << endl;
		exit(EXIT_FAILURE);
	}
}

void Bot::connect(string ip, int port)
{
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
	{
		cerr << "Invalid address or address not supported" << endl;
		exit(EXIT_FAILURE);
	}
	if (::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		cerr << "Connection error" << endl;
		exit(EXIT_FAILURE);
	}
}

void Bot::send(string msg)
{
	msg += "\n";
	if (::send(sock, msg.c_str(), msg.length(), 0) < 0)
	{
		cerr << "Send error" << endl;
		exit(EXIT_FAILURE);
	}
}

void Bot::read()
{
	int valread;
	if ((valread = ::read(sock, buffer, 1024)) < 0)
	{
		cerr << "Read error" << endl;
		exit(EXIT_FAILURE);
	}
	cout << buffer << endl;
}

// void Bot::hello(Channel &chan) {
// 	chan.chan_msg(*this, "Le bot " + get_nickname() + " est prêt !");
// }

// void Bot::welcome(Channel &chan, User &user) {
// 	// std::string chan = "";
// 	// for (int i = 0; i < 6; i++) {
// 	// 	chan += (char)(rand() % 26 + 'a');
// 	// }
// 	// return chan;
// 	chan.chan_msg(*this, ":" + this->get_nickname() + "!~" + this->get_username() + "@" +
// 		this->get_host() + " Salut " + user.get_nickname() + ", bienvenue sur le canal " + chan.getName() + " !");
// }

string Bot::get_nickname() { return name; }
