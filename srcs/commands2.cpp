#include "Server.hpp"

void Server::pass_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() != 2)
		return error_msg(user, "Wrong nember of args (PASS <password>)");
	if (user.is_logged())
		return error_msg(user, "You are already connected and cannot handshake again");
	else if (cmds[1] == password)
		user.set_pass(true);
	else
		user.set_pass(false);
}

void Server::nick_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() != 2)
		return error_msg(user, "Wrong nember of args (NICK <nickname>)");
	for (usr_it it = users.begin(); it != users.end(); it++)
		if (it->get_nickname() == cmds[1])
			return error_msg(user, "Nickname is already in use");
	user.set_nickname(cmds[1]);
}

void Server::user_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() != 5)
		return error_msg(user, "Wrong nember of args (USER <username> <host> <server name> <real name>)");
	if (user.is_logged())
		return error_msg(user, "You are already connected and cannot handshake again");
	user.set_username(cmds[1], cmds[2], cmds[3], cmds[4]);
}

