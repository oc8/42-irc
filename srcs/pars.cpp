#include "Server.hpp"

std::vector<string> split(string str, string delimiter);

void Server::parsing(string buffer, User &user)
{
	std::cout << "line = " << buffer << std::endl;
	std::vector<string> cmds = ::split(buffer, " ");
	// for (size_t i = 0; i < cmds.size(); i++)
	// 	std::cout << "cmds[" << i << "] = " << cmds[i] << std::endl;
	// std::cout << "cmds.size = " << cmds.size() << std::endl;
	string cmd = cmds[0];
	for (size_t i = 0; i < cmd.length(); i++)
		cmd[i] = std::tolower(cmd[i]);
	if (cmd[cmd.size() - 1] == '\n')
		cmd.erase(cmd.size() - 1);
	// cout << cmd << endl;
	
	if (!cmd.compare("pass"))
		return pass_cmd(user, cmds);
	else if (!cmd.compare("nick"))
		return nick_cmd(user, cmds);
	else if (!cmd.compare("user"))
		return user_cmd(user, cmds);
	else if (!cmd.compare("cap"))
		return ;
	else if (!user.is_logged())
		// return error_msg(user, "You are not connected to the server.");
		return send_msg(user, ":localhost 451 * :You have not registered");
	else if (!cmd.compare("join"))
		return join_cmd(user, cmds);
	else if (!cmd.compare("part"))
	{
	}
	else if (!cmd.compare("mode"))
	{
	}
	else if (!cmd.compare("topic"))
	{
	}
	else if (!cmd.compare("names"))
	{
	}
	else if (!cmd.compare("list"))
	{
	}
	else if (!cmd.compare("invite"))
	{
	}
	else if (!cmd.compare("kick"))
	{
	}
	else if (!cmd.compare("privmsg"))
		privmsg_cmd(user, cmds);
	else if (!cmd.compare("help"))
	{
	}
	else if (!cmd.compare("quit"))
	{
	}
	else if (!cmd.compare("ping"))
		ping_cmd(user, cmds);
	else
	{
		return error_msg(user, "Invalid command");
	}
}
