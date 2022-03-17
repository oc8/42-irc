#include "Server.hpp"

std::vector<string> split(string str, string delimiter);

void Server::parsing(string buffer, User &user)
{
	std::cout << buffer << std::endl;
	std::vector<string> split = ::split(buffer, " ");
	// for (size_t i = 0; i < split.size(); i++)
	// 	std::cout << "split[" << i << "] = " << split[i] << std::endl;
	// std::cout << "split.size = " << split.size() << std::endl;
	string cmd = split[0];
	for (size_t i = 0; i < cmd.length(); i++)
		cmd[i] = std::tolower(cmd[i]);
	if (cmd[cmd.size() - 1] == '\n')
		cmd.erase(cmd.size() - 1);
	for (size_t i = 0; i < cmds.size(); i++) {
		string &current = cmds[i].first;
		if (!user.is_logged() && current != "pass" && current != "user" && current != "nick" && current != "cap")
			return send_msg(user, ":localhost 451 * :You have not registered");
		if (cmd == current)
			return (this->*cmds[i].second)(user, split);
	}
	return send_msg(user, ":localhost 421 " + user.get_nickname() + " :Unknown command");
}
