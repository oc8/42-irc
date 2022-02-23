#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include "user.hpp"

class channel {
	public:
		typedef std::map<std::string, user>		list;

		//		--> CONSTRUCTORS/DESTRUCTORS <--
		
		channel(void) : n_user(0), users(list()) {}
		channel(const channel & src) {}
		~channel() {};


		//		--> GETTER/SETTERS <--

		std::string getName() { return name; }


		//		--> MEMBER FUCNTIONS <--

		bool add_ser(user newUser) { return users.insert(std::make_pair(newUser.getUserName(), newUser)).second; }
		bool del_user(user kicked) { return users.erase(kicked.getUserName()); }
		void sendMessage(std::string message) {}


		//		--> OPERATORS <--
		
		channel operator=(const channel & src);


	protected:
		int				n_user;
		int				max_user;
		std::string		name;
		list			users;
		std::string		password;
		std::string		mode;
		std::string		topic;
		bool			topic_modif_ope;
		bool			avail_invit;
		list			banned;
};


#endif