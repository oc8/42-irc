#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

class User {
	public:
		// typedef T											value_type;

		//		--> CONSTRUCTORS/DESTRUCTORS <--
		
		User(void) {}
		User(const User & src) {}
		~User() {};


		//		--> GETTER/SETTERS <--

		std::string getUserName() { return userName; }
		int getSD() { return sd; }


		//		--> MEMBER FUCNTIONS <--



		//		--> OPERATORS <--

		User operator=(const User & src);
		
	protected:
		int				sd;
		std::string		userName;
		std::string		nickName;
		Channel			*cur_chan;
		bool			operat;
		bool			visible;
		bool			notif_serv;
		bool			wallops;
};


#endif