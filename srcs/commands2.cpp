#include "server.hpp"

void Server::pass_cmd(User user, std::string pass) {
	if (user.is_logged()) {
		send(user.get_sd(), "You are already connected and cannot handshake again", strlen("You are already connected and cannot handshake again"), 0);
		return;
	}
	if (pass == password) {
		user.set_pass(true);
		
	}
	else
		user.set_pass(false);
}