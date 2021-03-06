#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <vector>
#include <list>

#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define RESET   "\033[0m"

using std::cerr;
using std::cout;
using std::endl;
using std::list;
using std::pair;
using std::string;
using std::vector;

#include "User.hpp"
#include "Channel.hpp"

class Server;
typedef void (Server::*pf)(User &user, std::vector<string> cmds);

class Server
{
public:
	typedef User *usr_ptr;
	typedef User &usr_ref;
	typedef Channel *chan_ptr;
	typedef Channel &chan_ref;
	typedef std::list<User>::iterator usr_it;
	typedef std::list<Channel>::iterator chan_it;

	//		--> CONSTRUCTORS/DESTRUCTORS <--

	Server(int port, string password);
	virtual ~Server();

	//		--> MEMBER FUCNTIONS <--

	void socket();
	void setsockopt();
	void bind();
	void listen();
	void accept();
	void read();
	void parsing(string buffer, User &user);
	void send_msg(User &user, std::string message);
	chan_it chan_exist(std::string chanName);
	usr_ptr user_exist(std::string userName);
	bool is_user(std::string userName);
	void erase_user_in_chans(User &user);
	std::list<Channel>::iterator is_channel(std::string chan);

	//		--> COMMANDS <--

	void pass_cmd(User &user, std::vector<string> cmds);
	void nick_cmd(User &user, std::vector<string> cmds);
	void user_cmd(User &user, std::vector<string> cmds);
	void join_cmd(User &user, std::vector<string> cmds);
	void mode_cmd(User &user, std::vector<string> cmds);
	void privmsg_cmd(User &user, std::vector<string> cmds);
	void notice_cmd(User &user, std::vector<string> cmds);
	void ping_cmd(User &user, std::vector<string> cmds);
	void kick_cmd(User &user, std::vector<string> cmds);
	void names_cmd(User &user, std::vector<string> cmds);
	void part_cmd(User &user, std::vector<string> cmds);
	void topic_cmd(User &user, std::vector<string> cmds);
	void invite_cmd(User &user, std::vector<string> cmds);
	void quit_cmd(User &user, vector<string> cmds);

private:
	int port;
	string password;
	int opt;
	int master_socket;
	int addrlen;
	int new_socket;
	std::vector<std::pair<string, pf> > cmds;
	struct sockaddr_in address;
	std::list<User> users;
	std::list<Channel> channels;
	fd_set readfds;
	usr_it user_it;
};
