#include "Server.hpp"

std::vector<string> split(string str, string delimiter);

void Server::join_cmd(User &user, std::vector<string> cmds) {
	std::vector<string> chan_name = split(cmds[1], ",");

	if (cmds.size() > 3)
		return(error_msg(user, "Too many arguments\n"));
	else if (cmds.size() < 2)
		return(error_msg(user, "Too few arguments\n"));
	for (std::vector<string>::iterator chan_name_it = chan_name.begin(); chan_name_it != chan_name.end(); ++chan_name_it)
	{
		bool exist = false;
		for (chan_it it = channels.begin(); it != channels.end(); it++)
		{
			if (it->getName() == *chan_name_it)
			{
				exist  = true;
				if (it->is_in_channel(user))
				{
					error_msg(user, "you're already in channel\n");
					break ;
				}
				if (it->getAvail_invit() == false)
				{
					it->add_user(&user);
					return_msg(user,  "Welcome to " + *chan_name_it + "\n Topic : " + it->getTopic() + "\n Users : " + it->nameUsers() );
				}
				else
					error_msg(user, *chan_name_it + " :Cannot join channel (+i)\n");
			}
		}
		if (exist == false)
		{
			channels.push_back(Channel(*chan_name_it, &user));
			return_msg(user,  "Welcome to " + *chan_name_it);
		}
	}
}
