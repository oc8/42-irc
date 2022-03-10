#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include "User.hpp"

class Channel {
	public:
		typedef User*					usr_ptr;
		typedef User&					usr_ref;
		typedef std::vector<usr_ptr>	list;
		typedef list::iterator			user_ptr_it;

		//		--> CONSTRUCTORS/DESTRUCTORS <--

		Channel(void);
		Channel(std::string name);
		Channel(std::string name, std::string psw);
		Channel(const Channel & src);
		virtual ~Channel();


		//		--> GETTER/SETTERS <--

		std::string getName();
		size_t getNbUser();
		size_t getNbOper();
		size_t getNbTot();

		bool setName(usr_ptr usr, std::string newName);
		bool setPsw(usr_ptr usr, std::string newName);
		bool setMode(usr_ptr usr, std::string newMode);
		bool setTopic(usr_ptr usr, std::string newMode);
		bool setAvailability(usr_ptr usr, bool availability);


		//		--> MEMBER FUCNTIONS <--

		void add_user(usr_ptr newUser, list &rank);
		void del_user(usr_ptr kicked, list &rank);
		void sendMessage(const char *message);
		bool is_operator(usr_ptr usr);
		bool invitation(usr_ptr inviter, usr_ptr usr);
		// bool setTopic(User us, std::string topic);


		//		--> OPERATORS <--
		
		Channel &operator=(const Channel & src);


	protected:
		std::string		name;
		list			users;
		list			operators;
		int				max_user;
		std::string		psw;
		std::string		mode;
		std::string		topic;
		bool			topic_modif_ope;
		bool			avail_invit;
		list			banned;
};


#endif
