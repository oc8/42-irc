#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include "user.hpp"

class channel {
	public:
		typedef std::map<std::string, user>		list;
		typedef list::iterator					user_it;

		//		--> CONSTRUCTORS/DESTRUCTORS <--
		
		channel(void) : n_user(0), users(list()) {}
		channel(const channel & src) {}
		~channel() {};


		//		--> GETTER/SETTERS <--

		std::string getName() { return name; }


		//		--> MEMBER FUCNTIONS <--

		bool add_user(user newUser);
		bool del_user(user kicked);
		void sendMessage(const char *message);
		bool setTopic(user us, std::string topic);


		//		--> OPERATORS <--
		
		channel operator=(const channel & src);


	protected:
		int				n_user;
		int				max_user;
		std::string		name;
		list			users;
		list			operators;
		std::string		password;
		std::string		mode;
		std::string		topic;
		bool			topic_modif_ope;
		bool			avail_invit;
		list			banned;
};


#endif