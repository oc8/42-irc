#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include "user.hpp"

class Channel {
	public:
		typedef User*							usr_ptr;
		typedef User*							usr_ref;
		typedef std::map<std::string, usr_ptr>	list;
		typedef list::iterator					user_it;

		//		--> CONSTRUCTORS/DESTRUCTORS <--
		
		Channel(void) {}
		Channel(std::string name) : name(name) {}
		Channel(std::string name, std::string psw) : name(name), psw(psw) {}
		Channel(const Channel & src) {}
		~Channel() {};


		//		--> GETTER/SETTERS <--

		std::string getName();
		size_t getNbUser();
		size_t getNbOper();
		size_t getNbTot();

		bool setName(usr_ptr usr, std::string newName);


		//		--> MEMBER FUCNTIONS <--

		bool add_user(usr_ptr newUser, list &rank);
		bool del_user(usr_ptr kicked, list &rank);
		void sendMessage(const char *message);
		bool is_operator(usr_ptr usr);
		// bool setTopic(User us, std::string topic);


		//		--> OPERATORS <--
		
		Channel operator=(const Channel & src);


	protected:
		std::string		name;
		list			users;
		list			operators;
		// int				max_user;
		std::string		psw;
		std::string		mode;
		std::string		topic;
		bool			topic_modif_ope;
		bool			avail_invit;
		list			banned;
};


#endif