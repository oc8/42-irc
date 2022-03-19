#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	  //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <vector>
#include <list>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::pair;

#include "User.hpp"
#include "Channel.hpp"
#include "Bot.hpp"

class Server;
typedef void (Server::*pf)(User &user, std::vector<string> cmds);

class Server {
	public:
		typedef User*							usr_ptr;
		typedef User&							usr_ref;
		typedef Channel*						chan_ptr;
		typedef Channel&						chan_ref;
		typedef std::list<User>::iterator		usr_it;
		typedef std::list<Channel>::iterator	chan_it;

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
		void erase_user_in_chans(User &user);


		//		--> COMMANDS <--

		void pass_cmd(User &user, std::vector<string> cmds);
		void nick_cmd(User &user, std::vector<string> cmds);
		void user_cmd(User &user, std::vector<string> cmds);
		void join_cmd(User &user, std::vector<string> cmds);
		void privmsg_cmd(User &user, std::vector<string> cmds);
		void ping_cmd(User &user, std::vector<string> cmds);
		void kick_cmd(User &user, std::vector<string> cmds);
		// void names_cmd(User &user, std::vector<string> cmds);
		void part_cmd(User &user, std::vector<string> cmds);

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
		Bot botch;
};
