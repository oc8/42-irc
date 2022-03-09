#include "server.hpp"
#include <vector>

std::vector<string> split(string str, string delimiter);

void Server::parsing(string buffer) {
	std::vector<string> cmds = ::split(buffer, " ");
	string cmd = cmds[0];
	for (size_t i = 0; i < cmd.length(); i++)
		cmd[i] = std::tolower(cmd[i]);
	cout << cmd << endl;
	if (cmd.compare("join")) {

	}
	else if (cmd.compare("part")) {

	}
	else if (cmd.compare("mode")) {

	}
	else if (cmd.compare("topic")) {

	}
	else if (cmd.compare("names")) {

	}
	else if (cmd.compare("list")) {

	}
	else if (cmd.compare("invite")) {

	}
	else if (cmd.compare("kick")) {

	}
	else if (cmd.compare("privmsg")) {

	}
	else if (cmd.compare("help")) {

	}
	else if (cmd.compare("quit")) {

	}
	// else {
	// 	"Invalid command";
	// }
}
