#ifndef USER_HPP
# define USER_HPP

#include <iostream>

class user {
	public:
		// typedef T											value_type;

		//		--> CONSTRUCTORS/DESTRUCTORS <--
		
		user(void) {}
		user(const user & src) {}
		~user() {};


		//		--> GETTER/SETTERS <--

		std::string getUserName() { return userName; }


		//		--> MEMBER FUCNTIONS <--



		//		--> OPERATORS <--

		user operator=(const user & src);
		
	protected:
		std::string		userName;
		std::string		nickName;
		bool			operat;
		bool			visible;
		bool			notif_serv;
		bool			wallops;
};


#endif