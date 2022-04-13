#include "Server.hpp"

std::vector<string> split(string str, string delimiter);

void Server::pass_cmd(User &user, std::vector<string> cmds) {
	if (user.is_logged())
		return send_msg(user, ":localhost 462 " + user.get_nickname() + " :You are already connected and cannot handshake again");
	if (cmds.size() != 2)
		return send_msg(user, ":localhost 461 " + user.get_nickname() + " :Not enough parameters");
	user.set_pass(cmds[1], password);
}

void Server::nick_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() == 1)
		return send_msg(user, ":localhost 431 " + user.get_nickname() + " :No nickname given");
	for (usr_it it = users.begin(); it != users.end(); it++) {
		if (it->get_nickname() == cmds[1])
			return send_msg(user, ":localhost 433 * " + cmds[1] + " :Nickname is already in use.");
	}
	user.set_nickname(cmds[1]);
}

void Server::user_cmd(User &user, std::vector<string> cmds) {
	if (user.is_logged())
		return send_msg(user, ":localhost 462 " + user.get_nickname() + " :You are already connected and cannot handshake again");
	if (cmds.size() < 5)
		return send_msg(user, ":localhost 461 " + user.get_nickname() + " :Not enough parameters");
	user.set_username(cmds[1], cmds[2], cmds[3], cmds[4]);
}

void Server::privmsg_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() == 1)
		 return send_msg(user, ":localhost 411 " + user.get_nickname() + " :No recipient given (PRIVMSG)");
	std::vector<std::string> targets = split(cmds[1], ",");
	std::string message;
	for (std::vector<std::string>::iterator it = cmds.begin() + 2; it != cmds.end(); it++) {
		message += *it;
		if (it + 1 != cmds.end())
			message += " ";
	}
	// std::cout << "message = \"" << message << "\"" << std::endl;
	if (message.empty())
		 return send_msg(user, ":localhost 412 " + user.get_nickname() + " :No text to send");
	if (message[0] != ':')
		message = (":" + message);
	// for (std::vector<std::string>::iterator it = targets.begin() + 2; it != targets.end(); it++) {
	for (size_t i = 0; i < targets.size(); i++) {
		if (targets[i][0] == '#' || targets[i][0] == '&') {
			chan_it chan = chan_exist(targets[i]);
			if (chan == channels.end())
				send_msg(user, ":localhost 401 " + user.get_nickname() + " " + targets[i] + " :No such nick/channel");
			else
			{
				(*chan).chan_msg_prv(user, ":" + user.get_nickname() + "!~" + user.get_username()
					+ "@" + user.get_host() + " PRIVMSG " + chan->getName() + " " + message);

			}
		}
		else {
			usr_ptr usr = user_exist(targets[i]);
			if (usr == NULL)
				// return_msg(user, targets[i] + " :No such nick/channel", 401);
				send_msg(user, ":localhost 401 " + user.get_nickname() + " " + targets[i] + " :No such nick/channel");
			else {
				send_msg(*usr, ":" + user.get_nickname() + "!~" + user.get_username()
					+ "@" + user.get_host() + " PRIVMSG " + (*usr).get_nickname() + " " + message);
			}
		}
	}
}

void Server::notice_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() == 1)
		return;
	std::vector<std::string> targets = split(cmds[1], ",");
	std::string message;
	for (std::vector<std::string>::iterator it = cmds.begin() + 2; it != cmds.end(); it++) {
		message += *it;
		if (it + 1 != cmds.end())
			message += " ";
	}
	if (message.empty())
		 return;
	if (message[0] != ':')
		message = (":" + message);
	for (size_t i = 0; i < targets.size(); i++) {
		if (targets[i][0] == '#' || targets[i][0] == '&') {
			chan_it chan = chan_exist(targets[i]);
			if (chan != channels.end())
				(*chan).chan_msg_prv(user, ":" + user.get_nickname() + "!~" + user.get_username()
					+ "@" + user.get_host() + " NOTICE " + chan->getName() + " " + message);

		}
		else {
			usr_ptr usr = user_exist(targets[i]);
			if (usr != NULL)
				send_msg(*usr, ":" + user.get_nickname() + "!~" + user.get_username()
					+ "@" + user.get_host() + " NOTICE " + (*usr).get_nickname() + " " + message);
		}
	}
}

void Server::ping_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() < 2)
		return send_msg(user, ":localhost 461 " + user.get_nickname() + " PING :Not enougt parameters");
	send_msg(user, ":localhost PING localhost :" + cmds[1] + "\n");
}
