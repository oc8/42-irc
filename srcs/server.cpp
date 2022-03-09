#include "server.hpp"

Server::Server(int port, string password)
{
    this->port = port;
    this->password = password;
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);
    this->addrlen = sizeof(address);
    this->opt = 1;
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

void Server::read()
{
    std::vector<int> client_socket;
    int activity, valread, sd, max_sd;
    unsigned long i;
    fd_set readfds;
    char message[] = ":localhost 001 ircserv :Welcome! \r\n";
    char buffer[1025];
    while (42)
    {
        // clear the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // add child sockets to set
        for (i = 0; i < client_socket.size(); i++)
        {
            // socket descriptor
            sd = client_socket[i];

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
            if (send(new_socket, message, strlen(message), 0) != static_cast<ssize_t>(strlen(message))) {
                cerr << "send" << endl;
                exit(EXIT_FAILURE);
            }
            cout << "Welcome message sent successfully" << endl;
            // add new socket to array of sockets
            client_socket.push_back(new_socket);
        }

        // else its some IO operation on some other socket
        for (i = 0; i < client_socket.size(); i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
                // Check if it was for closing , and also read the
                // incoming message
                if ((valread = ::read(sd, buffer, 1024)) == 0)
                {
                    // Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);
                    cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) << ", port " << ntohs(address.sin_port) << endl;

                    // Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                }
                // Echo back the message that came in
                else
                {
                    // set the string terminating NULL byte on the end
                    // of the data read
                    buffer[valread] = '\0';
                    printf("%s", buffer);
                    // send(sd, buffer, strlen(buffer), 0);
                    // this->parsing(buffer);
                }
            }
        }
    }
}
