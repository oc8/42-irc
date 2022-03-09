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
#include "user.hpp"
#include "channel.hpp"
#include <vector>
#include <map>

#include "user.hpp"
#include "channel.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

class Server {
	public:

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


		//		--> COMMANDS <--

		void pass_cmd(User user, std::string pass);


	private:
		int port;
		string password;
		int opt;
		int master_socket;
		int addrlen;
		int new_socket;
		struct sockaddr_in address;
		std::vector<User> users;
		std::vector<Channel> channels;
};
