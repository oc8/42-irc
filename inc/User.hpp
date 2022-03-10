#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

typedef struct connexion {
	bool pass;
	bool nick;
	bool user;
	bool connected;
} connexion_t;

class User {
	public:
		// typedef T											value_type;

		//		--> CONSTRUCTORS/DESTRUCTORS <--

		User();
		User(int sd);
		User(const User & src);
		virtual ~User();


		//		--> GETTERS <--

		std::string get_username() ;
		std::string get_nickname() ;
		int get_sd() ;
		bool is_logged() ;


		//		--> SETTERS <--

		void set_pass(bool is_ok);
		void set_nickname(std::string new_nickname);
		void set_username(std::string new_username);
		void setSD(int sd) { this->sd = sd; };


		//		--> MEMBER FUNCTIONS <--

		void init_user();
		void check_if_connected();


		//		--> OPERATORS <--

		User &operator=(const User & src);

	protected:
		int				sd;
		std::string		nickname;
		std::string		username;
		// bool			operat;
		bool			visible;
		bool			notif_serv;
		bool			wallops;
		connexion_t		connexion;
};


#endif
