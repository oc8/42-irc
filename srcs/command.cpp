#include "Server.hpp"

vector<string> split(string str, string delimiter);


void join_msg(User &user, Channel &chan, Server &serv) {
	// serv.send_msg(user, ":" + user.get_nickname() + "!~" + user.get_username()
	// 	+ "@" + user.get_host() + " JOIN " + chan.getName());
	serv.send_msg(user, ":localhost 331 " + user.get_nickname() + " " + chan.getName() + " :No topic is set");
	// serv.send_msg(user, ":localhost 332 " + chan.getName() + " +Cnst");
	serv.send_msg(user, ":localhost 353 " + user.get_nickname() + " @ " + chan.getName() + " :" + chan.nameOpe() + " " + chan.nameUsers());
	serv.send_msg(user, ":localhost 366 " + user.get_nickname() + " " + chan.getName() + " :End of /NAMES list.");
}

void Server::join_cmd(User &user, vector<string> cmds)
{
	vector<string> chan_name = split(cmds[1], ",");
	size_t num_chan = 0;

	if (cmds.size() < 2)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " JOIN :Not enough parameters"));
	for (vector<string>::iterator chan_name_it = chan_name.begin(); chan_name_it != chan_name.end(); ++chan_name_it)
	{
		bool exist = false;
		if ((*chan_name_it)[0] == '#' || (*chan_name_it)[0] == '&')
		{
			for (chan_it it = channels.begin(); it != channels.end(); it++)
			{
				if (it->getName() == *chan_name_it)
				{
					exist = true;
					if (it->is_banned(user))
					{
						send_msg(user, ":localhost 474 " + user.get_nickname() + " " + *chan_name_it + " :Cannot join channel (+b)");
						break;
					}
					if (it->is_in_channel(user))
						break;
					if (!it->good_pswd(cmds, num_chan))
					{
						send_msg(user, ":localhost 475 " + user.get_nickname() + " " + *chan_name_it + " :Cannot join channel (+k)");
						break;
					}
					if (it->getMaxUser() > 0 && it->getNbTot() == (size_t)it->getMaxUser())
					{
						send_msg(user, ":localhost 471 " + user.get_nickname() + " " + *chan_name_it + " :Cannot join channel (+l)");
						break;
					}
					if (it->getAvail_invit() == false)
					{
						it->add_user(&user);
						it->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@" + user.get_host() + " JOIN " + it->getName());
						join_msg(user, *it, *this);
					}
					else
						send_msg(user, ":localhost 473 " + user.get_nickname() + " " + *chan_name_it + " :Cannot join channel (+i)");
				}
			}
			if (exist == false)
			{
				channels.push_back(Channel(*chan_name_it));
				channels.back().add_ope(&user);
				channels.back().chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@" + user.get_host() + " JOIN " + channels.back().getName());
				join_msg(user, channels.back(), *this);
			}
		}
		else
			send_msg(user, ":localhost 403 " + user.get_nickname() + " " + *chan_name_it + " :No such channel");
		num_chan++;
	}
}

void Server::kick_cmd(User &user, vector<string> cmds)
{
	if (cmds.size() < 3)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " KICK :Not enough parameters\n"));

}

void Server::names_cmd(User &user, vector<string> cmds)
{
	// if (cmds.size() < 2) {
	// 	for (chan_it it = channels.begin(); it != channels.end(); it++)
	// 	{
	// 		string msg = ":localhost 353 " + user.get_nickname() + " = #" + it->getName() + " :";
	// 		for (list<usr_ptr>::iterator it_user = it->getUsers().begin(); it_user != it->getUsers().end(); it_user++)
	// 			msg += (*it_user)->get_nickname() + " ";
	// 		for (list<usr_ptr>::iterator it_user = it->getOpe().begin(); it_user != it->getOpe().end(); it_user++)
	// 			msg += (*it_user)->get_nickname() + " ";
	// 		send_msg(user, msg);
	// 	}
	// 	return (send_msg(user, ":localhost 366 " + user.get_nickname() + " :End of /NAMES list."));
	// }
	for (size_t i = 1; i < cmds.size(); i++)
	{
		chan_it it = chan_exist(cmds[i]);
		if (it != channels.end())
		{
			string msg = ":localhost 353 " + user.get_nickname() + " = " + it->getName() + " :" + user.get_nickname() + " ";
			list<User *> users = it->getUsers();
			for (list<User *>::iterator it = users.begin(); it != users.end(); it++)
				msg += (*it)->get_nickname() + " ";
			users = it->getOpe();
			for (list<User *>::iterator it = users.begin(); it != users.end(); it++)
				msg += (*it)->get_nickname() + " ";
			send_msg(user, msg);
			send_msg(user, ":localhost 366 " + user.get_nickname() + " " + it->getName() + " :End of /NAMES list.");
		}
		else
			send_msg(user, ":localhost 403 " + user.get_nickname() + " " + cmds[i] + " :No such channel");
	}
}

void pars_mode(std::string param, char sign, std::list<std::string> *mode){
	if ((sign == '+' && param[0] == '-') || (sign == '-' && param[0] == '+'))
		return pars_mode(param.substr(1), param[0], mode);
	else if (sign == param[0])
		return pars_mode(param.substr(1), sign, mode);
	if (param[0] == '\0')
		return;
	mode->push_back(std::string() + sign + param[0]);
	pars_mode(param.substr(1), sign, mode);
}



void Server::mode_cmd(User &user, std::vector<string> cmds){
	if (cmds.size() < 2)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " MODE :Not enough parameters"));
	std::list<Channel>::iterator chan_it;
	if ((chan_it = is_channel(cmds[1])) == channels.end())
		return (send_msg(user, ":localhost 403 " + user.get_nickname() + " " + cmds[1] + " :No such channel"));
	std::list<std::string> mode;
	pars_mode(cmds[2], '+', &mode);
	if (!chan_it->verif_mode(mode, user))
		return;
	std::list<std::string> ret;
	chan_it->exec_mode(user, mode, cmds, &ret);
	if (ret.size() >= 1)
		send_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@"
			+ user.get_host() + " MODE " + chan_it->getName() + " " + chan_it->display_mode(ret));
}

// void Server::names_cmd(User &user, std::vector<string> cmds) {}
