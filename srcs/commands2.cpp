#include "server.hpp"

void Server::pass_cmd(User &user, std::vector<string> cmds) {
	if (cmds.size() != 2)
		return error_msg(user, "Wrong nember of args, need only one (PASS <password>)");
	if (user.is_logged())
		return error_msg(user, "You are already connected and cannot handshake again");
	else if (cmds[1] == password)
		user.set_pass(true);
	else
		user.set_pass(false);
}
