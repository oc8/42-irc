#include "Bot.hpp"

Bot::Bot(std::string name) : name(name), sock(0)
{
	for (size_t i = 0; i < 1024; i++)
		buffer[i] = '\0';
	msg = "";
}

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

void Bot::pars()
{
	string line = buffer;
	nickname_user = "";
	msg = "";
	if (line.find('~') != string::npos)
		nickname_user = line.substr(1, line.find("!") - 1);
	if (line.find("JOIN #bot") != string::npos)
		send("PRIVMSG #bot :Bienvenu sur mon canal " + nickname_user + ", je suis le bot " + name + " !");
	if (line.find("PRIVMSG #bot :") != string::npos)
	{
		msg = line.substr(line.find("PRIVMSG #bot :") + 13);
		for (size_t i = 0; i < msg.length(); i++)
			msg[i] = std::tolower(msg[i]);
	}
}

void Bot::reply()
{
	if (msg.empty())
		return;
	else if (msg.find("salut") != string::npos || msg.find("bonjour") != string::npos || msg.find("hello") != string::npos || msg.find("hey") != string::npos || msg.find("coucou") != string::npos)
		send("PRIVMSG #bot :Salut " + nickname_user + " !");
	else if (msg.find("au revoir") != string::npos || msg.find("aurevoir") != string::npos)
		send("PRIVMSG #bot :Au revoir " + nickname_user + " !");
	else if (msg.find("aide") != string::npos || msg.find("help") != string::npos)
		send("PRIVMSG #bot :Je suis un bot qui ne sait rien faire, mais je suis toujours en train de faire des choses.");
	else if (msg.find("qui es tu") != string::npos)
		send("PRIVMSG #bot :Je suis le bot " + name + " !");
	else if (msg.find("qui est ton createur") != string::npos)
		send("PRIVMSG #bot :Les plus fort, les plus beau, les plus intelligents... odroz-ba tsimon tdayde !");
	else if (msg.find("que fais tu") != string::npos || msg.find("tu fais quoi") != string::npos)
		send("PRIVMSG #bot :Rien comme tsimon");
	else if (msg.find("qui est ton meilleur ami") != string::npos)
		send("PRIVMSG #bot :" + nickname_user + " !");
	if (rand() % 10 == 0)
	{
		std::string chan = "";
		for (int i = 0; i < rand() % 15; i++)
			chan += (char)(rand() % 26 + 'a');
		send("PRIVMSG #bot :" + chan);
	}
}

string Bot::get_nickname() { return name; }
