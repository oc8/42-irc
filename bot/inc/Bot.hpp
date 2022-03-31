#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::cerr;

class Bot
{
public:
	Bot(std::string name);
	~Bot();

	string get_nickname();
	string get_msg();

	void socket();
	void connect(string ip, int port);
	void send(string msg);
	void read();
	void pars(string line);
	void reply();

	// void hello(Channel &chan);
	// void welcome(Channel &chan, User &user);

private:
	Bot() {};
	string name;
	struct sockaddr_in serv_addr;
	int sock;
	string msg;
	string nickname_user;
};
