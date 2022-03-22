#pragma once

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using std::cerr;

typedef struct s_server {
	string ip;
	int port;
	string password;
} t_server;

class Bot
{
public:
	Bot(std::string name, string ip, int port, string password);
	~Bot();

	// void hello(Channel &chan);
	// void welcome(Channel &chan, User &user);
private:
	Bot() {};
	string name;
	t_server server;
};
