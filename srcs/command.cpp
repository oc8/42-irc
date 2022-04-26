#include "Server.hpp"

vector<string> split(string str, string delimiter);

void join_msg(User &user, Channel &chan, Server &serv)
{
	if (chan.getTopic().empty())
		serv.send_msg(user, ":localhost 331 " + user.get_nickname() + " " + chan.getName() + " :No topic is set");
	else 
		serv.send_msg(user, ":localhost 332 " + user.get_nickname() + " " + chan.getName() + " :" + chan.getTopic());
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
					if (!it->getAvail_invit() || it->is_invit(user))
					{
						it->add_user(&user);
						it->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 JOIN " + it->getName());
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
				channels.back().chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 JOIN " + channels.back().getName());
				join_msg(user, channels.back(), *this);
			}
		}
		else
			send_msg(user, ":localhost 403 " + user.get_nickname() + " " + *chan_name_it + " :No such channel");
		num_chan++;
	}
}

void Server::names_cmd(User &user, vector<string> cmds)
{
	if (cmds.size() < 2)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " NAMES :Not enough parameters\n"));
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
			{
				msg += (*it)->get_nickname() + " ";
			}
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

void pars_mode(std::string param, char sign, std::list<std::string> *mode)
{
	if ((sign == '+' && param[0] == '-') || (sign == '-' && param[0] == '+'))
		return pars_mode(param.substr(1), param[0], mode);
	else if (sign == param[0])
		return pars_mode(param.substr(1), sign, mode);
	if (param[0] == '\0')
		return;
	mode->push_back(std::string() + sign + param[0]);
	pars_mode(param.substr(1), sign, mode);
}

void Server::mode_cmd(User &user, std::vector<string> cmds)
{
	if (cmds.size() < 2)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " MODE :Not enough parameters"));
	std::list<Channel>::iterator chan_it;
	if ((chan_it = is_channel(cmds[1])) == channels.end())
		return (send_msg(user, ":localhost 403 " + user.get_nickname() + " " + cmds[1] + " :No such channel"));
	if (cmds.size() == 2)
		return (send_msg(user, ":localhost 324 " + user.get_nickname() + " " + cmds[1] + " " + chan_it->display_mode()));
	std::list<std::string> mode;
	pars_mode(cmds[2], '+', &mode);
	if (!chan_it->verif_mode(mode, user))
		return;
	if (!chan_it->is_operator(&user))
		return (send_msg(user, ":localhost 482 " + user.get_nickname() + " " + chan_it->getName() + " :You're not channel operator"));
	std::list<std::string> ret;
	chan_it->exec_mode(user, mode, cmds, &ret);
	if (ret.size() >= 1)
		chan_it->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 MODE " + chan_it->getName() + " " + chan_it->display_ret(ret));
}

void Server::topic_cmd(User &user, std::vector<std::string> cmds)
{
	if (cmds.size() < 2)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " TOPIC :Not enough parameters"));
	chan_it it_chan = chan_exist(cmds[1]);
	if (it_chan == channels.end())
		return (send_msg(user, ":localhost 403 " + user.get_nickname() + " " + cmds[1] + " :No such channel"));
	if (cmds.size() == 2)
	{
		if (it_chan->getTopic().empty())
			return (send_msg(user, ":localhost 331 " + user.get_nickname() + " " + it_chan->getName() + " :No topic is set"));
		return (send_msg(user, ":localhost 332 " + user.get_nickname() + " " + it_chan->getName() + " :" + it_chan->getTopic()));
	}
	std::string message = cmds[2];
	if (message[0] == ':')
		message.erase(0,1);
	if (!it_chan->setTopic(&user, message) || it_chan->is_operator(&user))
		it_chan->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@" + user.get_host()
			+ " TOPIC " + it_chan->getName() + " :" + message);
	else
		send_msg(user, ":localhost 482 " + user.get_nickname() + " " + it_chan->getName() + " :You're not channel operator");
}

void Server::invite_cmd(User &user, std::vector<std::string> cmds)
{
	if (cmds.size() < 3)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " INVITE :Not enough parameters"));
	if (!is_user(cmds[1]))
		return (send_msg(user, ":localhost 401 " + user.get_nickname() + " " + cmds[1] + " :No such nick/channel"));
	chan_it it_chan = chan_exist(cmds[2]);
	if (it_chan == channels.end())
		return (send_msg(user, ":localhost 403 " + user.get_nickname() + " " + cmds[2] + " :No such channel"));
	if (!it_chan->is_in_channel(user))
		return (send_msg(user, ":localhost 442 " + user.get_nickname() + " " + cmds[2] + " :You're not on that channel"));
	if (it_chan->is_in_channel(*user_exist(cmds[1])))
		return (send_msg(user, ":localhost 443 " + user.get_nickname() + " " + cmds[1] + " " + cmds[2] + " :is already on channel"));
	if (!it_chan->is_operator(&user))
		return (send_msg(user, ":localhost 482 " + user.get_nickname() + " " + cmds[2] + " :You're not channel operator"));
	it_chan->getInvit().push_back(cmds[1]);
	send_msg(user, ":localhost 341 " + user.get_nickname() + " " + cmds[1] + " " + cmds[2]);
	send_msg(*user_exist(cmds[1]), ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 INVITE " + cmds[1] + " :" + cmds[2]);
}

void Server::part_cmd(User &user, std::vector<std::string> cmds)
{
	if (cmds.size() < 2)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " PART :Not enough parameters"));
	vector<string> chan_name = split(cmds[1], ",");
	chan_it it_chan;
	for (vector<string>::iterator it = chan_name.begin(); it != chan_name.end(); ++it)
	{
		it_chan = chan_exist(*it);
		if (it_chan == channels.end())
			send_msg(user, ":localhost 403 " + user.get_nickname() + " " + cmds[1] + " :No such channel");
		else if (!it_chan->is_in_channel(user))
			send_msg(user, ":localhost 442 " + user.get_nickname() + " " + cmds[1] + " :You're not on that channel");
		else
		{
			if (cmds.size() == 2)
				it_chan->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 PART " + it_chan->getName());
			else
			{
				std::string message = cmds[2];
				if (message[0] == ':')
					message.erase(0,1);
				it_chan->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 PART " + it_chan->getName() + " :" + message);
			}
			it_chan->del_member(&user);
			if (it_chan->getNbTot() == 0)
				channels.erase(it_chan);
		}
	}
}

void Server::kick_cmd(User &user, vector<string> cmds)
{
	if (cmds.size() < 3)
		return (send_msg(user, ":localhost 461 " + user.get_nickname() + " KICK :Not enough parameters"));
	chan_it it_chan = chan_exist(cmds[1]);
	if (it_chan == channels.end())
		return (send_msg(user, ":localhost 403 " + user.get_nickname() + " " + cmds[1] + " :No such channel"));
	if (!it_chan->is_in_channel(user))
		return (send_msg(user, ":localhost 442 " + user.get_nickname() + " " + cmds[1] + " :You're not on that channel"));
	if (!it_chan->is_operator(&user))
		return (send_msg(user, ":localhost 482 " + user.get_nickname() + " " + cmds[1] + " :You're not channel operator"));
	if (cmds.size() == 3)
		it_chan->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 KICK " + it_chan->getName() + " " + cmds[2] + " :" + cmds[2]);
	else
		it_chan->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 KICK " + it_chan->getName() + " " + cmds[2] + " :" + cmds[3]);
	it_chan->del_member(user_exist(cmds[2]));
}

void Server::quit_cmd(User &user, vector<string> cmds)
{
	(void)cmds;
	getpeername(user.get_sd(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    cout << BOLDYELLOW << "Host disconnected, ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << RESET << endl;
	send_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 QUIT :Client Quit");
	for (chan_it it = channels.begin(); it != channels.end(); it++) {
		it->del_user(&user);
		it->del_ope(&user);
		it->chan_msg(user, ":" + user.get_nickname() + "!~" + user.get_username() + "@127.0.0.1 QUIT :Client Quit");
		if (it->is_empty())
			it = channels.erase(it);
	}
	close(user.get_sd());
	FD_CLR(user.get_sd(), &readfds);
	user_it = users.erase(user_it);
}
