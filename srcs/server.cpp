#include "Server.hpp"
#define pair pair<string, pf>

Server::Server(int port, string password)
{
    this->port = port;
    this->password = password;
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);
    this->addrlen = sizeof(address);
    this->opt = 1;
    // botch = Bot("botch");
    cmds.push_back(pair("pass", &Server::pass_cmd));
    cmds.push_back(pair("nick", &Server::nick_cmd));
    cmds.push_back(pair("user", &Server::user_cmd));
    cmds.push_back(pair("join", &Server::join_cmd));
    cmds.push_back(pair("privmsg", &Server::privmsg_cmd));
    cmds.push_back(pair("ping", &Server::ping_cmd));
    cmds.push_back(pair("kick", &Server::kick_cmd));
    cmds.push_back(pair("names", &Server::names_cmd));
    cmds.push_back(pair("mode", &Server::mode_cmd));
    cmds.push_back(pair("topic", &Server::topic_cmd));
    cmds.push_back(pair("invite", &Server::invite_cmd));
}

Server::~Server() {}

void Server::socket()
{
    if ((this->master_socket = ::socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cerr << "socket failed" << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::setsockopt()
{
    if (::setsockopt(this->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&this->opt, sizeof(this->opt)) < 0)
    {
        cerr << "setsockopt" << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::bind()
{
    if (::bind(this->master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        cerr << "bind failed" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Listener on port " << this->port << endl;
}

void Server::listen()
{
    if (::listen(this->master_socket, 3) < 0)
    {
        cerr << "listen" << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::accept()
{
    this->addrlen = sizeof(this->address);
    if ((this->new_socket = ::accept(this->master_socket, (struct sockaddr *)&this->address, (socklen_t *)&this->addrlen)) < 0)
    {
        cerr << "accept" << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::erase_user_in_chans(User &user) {
    for (chan_it it = channels.begin(); it != channels.end(); it++) {
        it->del_user(&user);
        it->del_ope(&user);
        if (it->is_empty())
            it = channels.erase(it);
    }
}

string erase_str_in_str(string str, string erase);
std::vector<string> split(string str, string delimiter);

void Server::read()
{
    int activity, valread, sd, max_sd;
    // unsigned long i;
    fd_set readfds;
    // char message[] = ":localhost 001 ircserv :Welcome! \r\n";
    char buffer[1025];
    while (42) {
        // clear the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // add child sockets to set
        // for (i = 0; i < users.size(); i++)
        for (usr_it it = users.begin(); it != users.end(); it++)
        {
            // socket descriptor
            // sd = users[i].get_sd();
            sd = it->get_sd();

            // if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            // highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        // wait for an activity on one of the sockets , timeout is NULL ,
        // so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            cout << "select error" << endl;
            exit(EXIT_FAILURE);
        }

        // If something happened on the master socket ,
        // then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = ::accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                cerr << "accept" << endl;
                exit(EXIT_FAILURE);
            }

            // inform user of socket number - used in send and receive commands
            cout << "New connection, socket fd is: " << new_socket << ", ip is: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << endl;

            // send new connection greeting message
            char message[] = ":localhost 300 * :To connect ircserv, please enter the password, your nickname and username\n";
            if (send(new_socket, message, strlen(message), 0) != static_cast<ssize_t>(strlen(message)))
            {
                cerr << "send" << endl;
                exit(EXIT_FAILURE);
            }
            cout << "Hello message sent successfully" << endl;
            // add new socket to array of sockets
            users.push_back(User(new_socket));
            // std::cout << "user.size = " << users.size() << std::endl;
            // for (i = 0; i < users.size(); i++)
            //     cout << "sd " << i << ": " << users[i].get_sd() << endl;
        }

        // else its some IO operation on some other socket
        // for (i = 0; i < users.size(); i++)
        for (usr_it it = users.begin(); it != users.end(); it++)
        {
            // sd = users[i].get_sd();
            sd = it->get_sd();
            if (FD_ISSET(sd, &readfds))
            {
                // Check if it was for closing , and also read the
                // incoming message
                if ((valread = ::read(sd, buffer, 1024)) == 0)
                {
                    // Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);
                    cout << "Host disconnected, ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << endl;

                    // Close the socket and mark as 0 in list for reuse
                    erase_user_in_chans(*it);
                    close(sd);
                    it = users.erase(it);
                }
                // Echo back the message that came in
                else
                {
                    // set the string terminating NULL byte on the end
                    // of the data read
                    buffer[valread] = '\0';
                    // send(sd, buffer, strlen(buffer), 0);
                    string buf = erase_str_in_str(buffer, "\r");
                    std::vector<string> lines = ::split(buf, "\n");
                    for (size_t j = 0; j < lines.size(); ++j)
                        // this->parsing(lines[j], users[i]);
                        parsing(lines[j], *it);
                }
            }
        }
    }
}

void Server::send_msg(User &user, std::string message) {
    send(user.get_sd(), (message + "\n").c_str(), message.size() + 1, 0);
}

Server::chan_it Server::chan_exist(std::string chanName) {
	for (chan_it it = channels.begin(); it != channels.end(); it++)
		if (it->getName() == chanName)
			return it;
	return channels.end();
}

User *Server::user_exist(std::string userName) {
	for (usr_it it = users.begin(); it != users.end(); it++)
		if (it->get_nickname() == userName)
			return &(*it);
	return NULL;
}

bool Server::is_user(std::string userName){
    for (usr_it it = users.begin(); it != users.end(); it++)
		if (it->get_nickname() == userName)
            return true;
    return false;
}

std::list<Channel>::iterator Server::is_channel(std::string chan){
    for (std::list<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		if ((*it).getName() == chan)
            return it;
    return channels.end();
}
