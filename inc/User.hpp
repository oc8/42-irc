#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

typedef struct connexion
{
	bool pass;
	bool nick;
	bool user;
	bool connected;
} connection_t;

class User
{
public:
	typedef User *usr_ptr;
	typedef User &usr_ref;

	//		--> CONSTRUCTORS/DESTRUCTORS <--

	User();
	User(int sd);
	User(const User &src);
	virtual ~User();

	//		--> GETTERS <--

	std::string get_username() const;
	std::string get_nickname() const;
	std::string get_host() const;
	int get_sd() const;
	bool is_logged() const;

	//		--> SETTERS <--

	void set_pass(std::string usr_pass, std::string serv_pass);
	void set_nickname(std::string new_nickname);
	void set_username(std::string new_username, std::string new_host,
							std::string new_servername, std::string new_realname);
	void setSD(int sd) { this->sd = sd; };

	//		--> MEMBER FUNCTIONS <--

	void init_user();
	void check_if_connected();

	//		--> OPERATORS <--

	User &operator=(const User &src);

protected:
	int sd;
	std::string nickname;
	std::string username;
	bool visible;
	bool notif_serv;
	bool wallops;
	connection_t connection;
	std::string host;
	std::string server_name;
	std::string real_name;
};

#endif
