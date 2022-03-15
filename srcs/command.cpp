#include "Server.hpp"

std::vector<string> split(string str, string delimiter);

void Server::join_cmd(User &user, std::vector<string> cmds) {
	std::vector<string> chan_name = split(cmds[1], ",");
	size_t num_chan = 0;

	if (cmds.size() < 2)
		return(error_msg(user, "JOIN :Not enough parameters\n"));
	for (std::vector<string>::iterator chan_name_it = chan_name.begin(); chan_name_it != chan_name.end(); ++chan_name_it)
	{
		bool exist = false;
		if ((*chan_name_it)[0] == '#' || (*chan_name_it)[0] == '&')
		{
			for (chan_it it = channels.begin(); it != channels.end(); it++)
			{
				if (it->getName() == *chan_name_it)
				{
					exist  = true;
					if (it->is_banned(user))
					{
						error_msg(user, *chan_name_it + " :Cannot join channel (+b)\n");
						break ;
					}
					if (it->is_in_channel(user))
						break ;
					if (!it->good_pswd(cmds, num_chan))
					{
						error_msg(user, *chan_name_it + " :Cannot join channel (+k)\n");
						break ;
					}
					if (it->getAvail_invit() == false)
					{
						it->add_user(&user);
						send_msg(user, it->nameOpe());
						send_msg(user, it->nameUsers());
					}
					else
						error_msg(user, *chan_name_it + " :Cannot join channel (+i)\n");
				}
			}
			if (exist == false)
			{
				channels.push_back(Channel(*chan_name_it));
				channels.back().add_ope(&user);
			}
		}
		else
			send_msg(user, *chan_name_it + " :No such channel\n");
		num_chan++;
	}
}
