#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include "user.hpp"

class Channel {
	public:
		typedef std::map<std::string, User>		list;
		typedef list::iterator					user_it;

		//		--> CONSTRUCTORS/DESTRUCTORS <--
		
		Channel(void) : n_user(0) {}
		Channel(const Channel & src) {}
		~Channel() {};


		//		--> GETTER/SETTERS <--

		std::string getName() { return name; }


		//		--> MEMBER FUCNTIONS <--

		bool add_user(User newUser);
		bool del_user(User kicked);
		void sendMessage(const char *message);
		bool setTopic(User us, std::string topic);


		//		--> OPERATORS <--
		
		Channel operator=(const Channel & src);


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