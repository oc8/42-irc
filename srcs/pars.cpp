#include "Server.hpp"

std::vector<string> split(string str, string delimiter);

void Server::parsing(string buffer, User &user) {
	std::vector<string> cmds = ::split(buffer, " ");
	string cmd = cmds[0];
	for (size_t i = 0; i < cmd.length(); i++)
		cmd[i] = std::tolower(cmd[i]);
	cmd.erase(cmd.size() - 1);
	// cout << cmd << endl;
	if (!cmd.compare("nick")) {
	}
	else if (!cmd.compare("user")) {
	}
	else if (!cmd.compare("pass")) {
		pass_cmd(user, cmds);
	}
	// else if (!user.is_logged()) {
	// 	this->error_msg(user, "You are not connected to the server.");
	// }
	else if (!cmd.compare("join")) {
	}
	else if (!cmd.compare("part")) {
	}
	else if (!cmd.compare("mode")) {
	}
	else if (!cmd.compare("topic")) {
	}
	else if (!cmd.compare("names")) {
	}
	else if (!cmd.compare("list")) {
	}
	else if (!cmd.compare("invite")) {
	}
	else if (!cmd.compare("kick")) {
	}
	else if (!cmd.compare("privmsg")) {
	}
	else if (!cmd.compare("help")) {
	}
	else if (!cmd.compare("quit")) {
	}
	// else {
	// 	"Invalid command";
	// }
}
