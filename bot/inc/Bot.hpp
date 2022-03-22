#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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

	void socket();
	void connect(string ip, int port);
	void send(string msg);
	void read();

	// void hello(Channel &chan);
	// void welcome(Channel &chan, User &user);

private:
	Bot() {};
	string name;
	struct sockaddr_in serv_addr;
	int sock;
	char buffer[1024];
};
