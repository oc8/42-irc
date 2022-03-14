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
		// typedef User*					usr_ptr;
		// typedef User&					usr_ref;
		typedef std::vector<usr_ptr>	list;
		typedef list::iterator			user_ptr_it;

		//		--> CONSTRUCTORS/DESTRUCTORS <--

		Channel(void);
		Channel(std::string name);
		Channel(std::string name, usr_ptr ope);
		Channel(std::string name, std::string psw);
		Channel(const Channel & src);
		virtual ~Channel();


		//		--> GETTER/SETTERS <--

		std::string getName();
		std::string getTopic();
		list getUsers();
		list getOpe();
		size_t getNbUser();
		size_t getNbOper();
		size_t getNbTot();
		bool getAvail_invit();

		bool setName(usr_ptr usr, std::string newName);
		bool setPsw(usr_ptr usr, std::string newName);
		bool setMode(usr_ptr usr, std::string newMode);
		bool setTopic(usr_ptr usr, std::string newMode);
		bool setAvailability(usr_ptr usr, bool availability);


		//		--> MEMBER FUCNTIONS <--

		void init_chan();
		void add_user(usr_ptr newUser);
		void del_user(usr_ptr kicked);
		void add_ope(usr_ptr newOpe);
		void del_ope(usr_ptr kickedOpe);
		void sendMessage(const char *message);
		bool is_operator(usr_ptr usr);
		bool invitation(usr_ptr inviter, usr_ptr usr);
		std::string nameUsers();
		bool is_in_channel(User &user);
		// bool setTopic(User us, std::string topic);


		//		--> OPERATORS <--
		
		Channel &operator=(const Channel & src);


	protected:
		std::string					_name;
		list						users;
		list						operators;
		int							max_user;
		std::string					psw;
		std::string					mode;
		std::string					topic;
		bool						topic_modif_ope;
		bool						avail_invit;
		list						banned;
};


#endif
