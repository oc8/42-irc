#include "server.hpp"
#include <vector>

std::vector<string> split(string str, string delimiter) {
	size_t pos = 0;
	std::vector<string> tokens;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		if (pos > 0)
			tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	return tokens;
}

void Server::parsing(char *buffer) {
	std::vector<string> cmds = ::split(buffer, "");
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
	// 	else {
	// 		return "Invalid command";
	// 	}
}
