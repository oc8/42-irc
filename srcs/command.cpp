#include "Server.hpp"

std::vector<string> split(string str, string delimiter);


void join_msg(User &user, Channel &chan, Server &serv) {
	serv.send_msg(user, ":" + user.get_nickname() + "!~" + user.get_username()
		+ "@" + user.get_host() + " JOIN " + chan.getName());
	serv.send_msg(user, ":localhost MODE " + chan.getName() + " +Cnst");
	serv.send_msg(user, ":localhost 353 " + user.get_nickname() + " @ " + chan.getName() + " :" + chan.nameOpe() + " " + chan.nameUsers());
	serv.send_msg(user, ":localhost 366 " + user.get_nickname() + " " + chan.getName() + " :End of /NAMES list.");
}

void Server::join_cmd(User &user, std::vector<string> cmds) {
	std::vector<string> chan_name = split(cmds[1], ",");
	size_t num_chan = 0;

	if (cmds.size() < 2)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " JOIN :Not enough parameters\n"));
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
						send_msg(user, ":localhost 474" + user.get_nickname() + *chan_name_it + " :Cannot join channel (+b)\n");
						break ;
					}
					if (it->is_in_channel(user))
						break ;
					if (!it->good_pswd(cmds, num_chan))
					{
						send_msg(user, ":localhost 475" + user.get_nickname() + *chan_name_it + " :Cannot join channel (+k)\n");
						break ;
					}
					if (it->getAvail_invit() == false)
					{
						it->add_user(&user);
						join_msg(user, *it, *this);
						it->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username()
							+ "@" + user.get_host() + " JOIN " + it->getName());
					}
					else
						send_msg(user, ":localhost 473" + user.get_nickname() + *chan_name_it + " :Cannot join channel (+i)\n");
				}
			}
			if (exist == false)
			{
				channels.push_back(Channel(*chan_name_it));
				channels.back().add_ope(&user);
				join_msg(user, channels.back(), *this);
			}
		}
		else
			send_msg(user, ":localhost 403" + user.get_nickname() + *chan_name_it + " :No such channel\n");
		num_chan++;
	}
}

// void Server::kick_cmd(User &user, std::vector<string> cmds) {}

// void Server::names_cmd(User &user, std::vector<string> cmds) {}
