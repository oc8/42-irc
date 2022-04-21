#include "Bot.hpp"
#include <math.h>

Bot::Bot(std::string name) : name(name), sock(0)
{
	msg = "";
}

Bot::~Bot() {}

void Bot::socket()
{
	if ((sock = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Socket creation error" << endl;
		exit(-1);
	}
}

void Bot::connect(string ip, int port)
{
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
	{
		cerr << "Invalid address or address not supported" << endl;
		exit(-1);
	}
	if (::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		cerr << "Connection error" << endl;
		exit(-1);
	}
}

void Bot::send(string msg)
{
	msg += "\n";
	if (::send(sock, msg.c_str(), msg.length(), 0) < 0)
	{
		cerr << "Send error" << endl;
		exit(-1);
	}
}

string erase_str_in_str(string str, string erase)
{
	size_t pos = 0;
	while ((pos = str.find(erase, pos)) != std::string::npos) {
		str.erase(pos, erase.length());
	}
	return str;
}
void Bot::read()
{
	int valread;
	char buffer[1024] = {0};
	if ((valread = ::read(sock, buffer, 1024)) < 0)
	{
		cerr << "Read error" << endl;
		exit(-1);
	}
	buffer[valread] = '\0';
	msg = buffer;
	msg = erase_str_in_str(msg, "\r");
}

void Bot::pars(string line)
{
	nickname_user = "";
	msg = "";
	if (line.find(":localhost") != string::npos)
		return ;
	if (line.find('~') != string::npos)
		nickname_user = line.substr(1, line.find("!") - 1);
	if (line.find("JOIN #bot") != string::npos && nickname_user != get_nickname() && nickname_user != "")
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
	if (msg.empty() || nickname_user == get_nickname())
		return;
	string reply = "PRIVMSG #bot :";
	if (msg.find("salut") != string::npos || msg.find("bonjour") != string::npos || msg.find("hello") != string::npos || msg.find("hey") != string::npos || msg.find("coucou") != string::npos)
		reply += "Salut " + nickname_user + " !";
	else if (msg.find("au revoir") != string::npos || msg.find("aurevoir") != string::npos)
		reply += "Au revoir " + nickname_user + " !";
	else if (msg.find("aide") != string::npos || msg.find("help") != string::npos)
		reply += "Je suis un bot qui ne sait rien faire, mais je suis toujours en train de faire des choses.";
	else if (msg.find("qui es tu") != string::npos)
		reply += "Je suis le bot " + name + " !";
	else if (msg.find("qui est ton createur") != string::npos)
		reply += "Les plus forts, les plus beaux, les plus intelligents... odroz-ba tsimon tdayde !";
	else if (msg.find("que fais tu") != string::npos || msg.find("tu fais quoi") != string::npos)
		reply += "Rien comme tsimon";
	else if (msg.find("qui est ton meilleur ami") != string::npos)
		reply += "" + nickname_user + " !";
	else if (msg.find("ca va") != string::npos)
		reply += "Tout va bien !";
	else if (msg.find("tg") != string::npos)
		return send("KICK #bot " + nickname_user);
	if (reply != "PRIVMSG #bot :")
		send(reply);
	else if (rand() % 10 == 0)
	{
		std::string chan = "";
		for (int i = 0; i < rand() % 15; i++)
			chan += (char)(rand() % 26 + 'a');
		send("PRIVMSG #bot :" + chan + " !");
	}
}

string Bot::get_nickname() { return name; }

string Bot::get_msg() { return msg; }

