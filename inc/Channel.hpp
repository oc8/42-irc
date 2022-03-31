#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include "User.hpp"

class Channel {
	public:
		typedef User*								usr_ptr;
		typedef User&								usr_ref;
		// typedef User*								usr_ptr;
		// typedef User&								usr_ref;
		typedef std::list<usr_ptr>				list;
		typedef list::iterator						user_ptr_it;
		typedef std::list<std::string>				ban_list;
		typedef std::list<std::string>::iterator	ban_it;

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
		std::list<std::string> getInvit();
		size_t getNbUser();
		size_t getNbOper();
		size_t getNbTot();
		int getMaxUser();
		bool getAvail_invit();

		bool setName(usr_ptr usr, std::string newName);
		bool setPsw(usr_ptr usr, std::string newName);
		bool setMode(usr_ptr usr, std::string newMode);
		bool setTopic(usr_ptr usr, std::string newMode);
		bool setAvailability(usr_ptr usr, bool availability);


		//		--> MEMBER FUCNTIONS <--

		void init_chan();
		void del_member(usr_ptr member);
		void add_user(usr_ptr newUser);
		void del_user(usr_ptr kicked);
		void add_ope(usr_ptr newOpe);
		void del_ope(usr_ptr kickedOpe);
		void chan_msg(usr_ref user, std::string message);
		void chan_msg_prv(usr_ref user, std::string message);
		void send_msg(User &user, std::string message);
		bool is_user(usr_ptr usr);
		bool is_operator(usr_ptr usr);
		bool is_empty();
		bool invitation(usr_ptr inviter, usr_ptr usr);
		std::string nameUsers();
		std::string nameOpe();
		bool is_in_channel(User &user);
		bool is_banned(User &user);
		bool is_invit(User &user);
		void ban_user(std::string nick);
		void deban_user(std::string nick);
		bool good_pswd(std::vector<std::string> cmds, size_t i);
		bool verif_mode(std::list<std::string> mode, User &user);
		void exec_mode(User &user, std::list<std::string> mode, std::vector<std::string> cmds, std::list<std::string> *ret);
		std::string display_mode(std::list<std::string> ret);
		// bool setTopic(User us, std::string topic);


		//		--> OPERATORS <--
		
		Channel &operator=(const Channel & src);

	protected:
		std::string		name;
		list			users;
		list			operators;
		std::list<std::string>			invit;
		int				max_user;
		std::string		psw;
		std::string		mode;
		std::string		topic;
		bool			topic_editable;
		bool			avail_invit;
		bool			exterior_msg;
		ban_list		banned;

	private:
		void invite_mode(char sign, std::list<std::string> *ret);
		void op_mode(User &user, char sign, std::vector<std::string> cmds, std::list<std::string> *ret);
		void limit_mode(User &user, char sign, std::vector<std::string> cmds, std::list<std::string> *ret);
		void psw_mode(char sign, std::vector<std::string> cmds, std::list<std::string> *ret);
		void topic_mode(char sign, std::list<std::string> *ret);
		void add_ret_mode(std::list<std::string> *ret, std::string mode, std::string target);
};


#endif
