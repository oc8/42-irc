#include "Server.hpp"

void Server::pass_cmd(User &user, std::vector<string> cmds) {
	if (user.is_logged())
		return error_msg(user, "You are already connected and cannot handshake again");
	if (cmds.size() != 2)
		return error_msg(user, "Wrong nember of args (PASS <password>)");
	user.set_pass(cmds[1], password);
}

void Server::nick_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() != 2)
		return error_msg(user, "Wrong nember of args (NICK <nickname>)");
	for (usr_it it = users.begin(); it != users.end(); it++) {
		if (it->get_nickname() == cmds[1])
			return error_msg(user, "Nickname is already in use");
	}
	user.set_nickname(cmds[1]);
}

void Server::user_cmd(User &user, std::vector<string> cmds) {
	if (user.is_logged())
		return error_msg(user, "You are already connected and cannot handshake again");
	if (cmds.size() != 5)
		return error_msg(user, "Wrong nember of args (USER <username> <host> <server name> <real name>)");
	user.set_username(cmds[1], cmds[2], cmds[3], cmds[4]);
}

void Server::join_cmd(User &user, std::vector<string> cmds) {
	for (chan_it it = channels.begin(); it != channels.end(); it++) {
		// std::cout << "exist nick = " << it->get_nickname() << "nick = " << cmds[1] << std::endl;
		if (it->getName() == cmds[1]) {
			it->add_user(&user);
			 return return_msg(user,  "Welcome to " + cmds[1] + "\n Topic : " + it->getTopic() + "\n Users : " + it->nameUsers() );
		}
	}
	channels.push_back(Channel(cmds[1], &user));
	return_msg(user,  "Welcome to " + cmds[1] );

	// if (user.is_logged())
	// 	return error_msg(user, "You are already connected and cannot handshake again");
	// if (cmds.size() != 5)
	// 	return error_msg(user, "Wrong nember of args (USER <username> <host> <server name> <real name>)");
	// user.set_username(cmds[1], cmds[2], cmds[3], cmds[4]);
}

