#include "server.hpp"

Server::Server(int port, string password) {
	this->port = port;
	this->password = password;
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);
	this->addrlen = sizeof(address);
	this->opt = TRUE;
}

Server::~Server() {}

void Server::socket() {
	if ((this->master_socket = ::socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		cerr << "socket failed" << endl;
		exit(EXIT_FAILURE);
	}
}

void Server::setsockopt() {
	if (::setsockopt(this->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&this->opt, sizeof(this->opt)) < 0) {
		cerr << "setsockopt" << endl;
		exit(EXIT_FAILURE);
	}
}

void Server::bind() {
	if (::bind(this->master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		cerr << "bind failed" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Listener on port " << this->port << endl;
}

void Server::listen() {
	if (::listen(this->master_socket, 3) < 0) {
		cerr << "listen" << endl;
		exit(EXIT_FAILURE);
	}
}

void Server::accept() {
	this->addrlen = sizeof(this->address);
	if ((this->new_socket = ::accept(this->master_socket, (struct sockaddr *)&this->address, (socklen_t *)&this->addrlen)) < 0) {
		cerr << "accept" << endl;
		exit(EXIT_FAILURE);
	}
}

void Server::read() {
	int max_clients = 30;
	int client_socket[30];
	for (int i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
	}
	int activity, valread, sd, max_sd, i;
	fd_set readfds;
	char message[] = "ECHO Daemon v1.0 \r\n";
	char buffer[1025];
	while (TRUE) {
		FD_ZERO(&readfds);
		FD_SET(this->master_socket, &readfds);
		max_sd = this->master_socket;
		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];
			if (sd > 0) {
				FD_SET(sd, &readfds);
			}
			if (sd > max_sd) {
				max_sd = sd;
			}
		}
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR)) {
			cerr << "select error" << endl;
			exit(EXIT_FAILURE);
		}
		if (FD_ISSET(this->master_socket, &readfds)) {
			if ((this->new_socket = ::accept(this->master_socket, (struct sockaddr *)&this->address, (socklen_t *)&this->addrlen)) < 0) {
				cerr << "accept" << endl;
				exit(EXIT_FAILURE);
			}
			for (i = 0; i < max_clients; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = this->new_socket;
					cout << "New connection , socket fd is " << this->new_socket << endl;
					break;
				}
			}
		}
		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];
			if (FD_ISSET(sd, &readfds)) {
				if ((valread = ::read(sd, buffer, 1024)) == 0)
				{
					getpeername(sd, (struct sockaddr*)&this->address, (socklen_t*)&this->addrlen);
					cout << "Host disconnected , ip " << inet_ntoa(this->address.sin_addr) << ", port " << ntohs(this->address.sin_port) << endl;
					close(sd);
					client_socket[i] = 0;
				}
				else {
					for (int j = 0; j < valread; j++) {
						buffer[j] = toupper(buffer[j]);
					}
					write(sd, buffer, valread);
				}
			}
		}
	}
	while (TRUE) {
		// clear the socket set
		FD_ZERO(&readfds);

		// add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		// add child sockets to set
		for (i = 0; i < max_clients; i++)
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

		if ((activity < 0) && (errno != EINTR)) {
			printf("select error");
		}

		// If something happened on the master socket ,
		// then its an incoming connection
		if (FD_ISSET(master_socket, &readfds)) {
			if ((new_socket = ::accept(master_socket,
											 (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
			cout << "New connection, socket fd is " << new_socket << ", ip is : " << inet_ntoa(address.sin_addr) << ", port : " << ntohs(address.sin_port) << endl;

			// send new connection greeting message
			if (send(new_socket, message, strlen(message), 0) != static_cast<ssize_t>(strlen(message))) {
				perror("send");
			}

			puts("Welcome message sent successfully");

			// add new socket to array of sockets
			for (i = 0; i < max_clients; i++) {
				// if position is empty
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n", i);

					break;
				}
			}
		}

		// else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds)) {
				// Check if it was for closing , and also read the
				// incoming message
				if ((valread = ::read(sd, buffer, 1024)) == 0) {
					// Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr *)&address,
									(socklen_t *)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
							 inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					// Close the socket and mark as 0 in list for reuse
					close(sd);
					client_socket[i] = 0;
				}

				// Echo back the message that came in
				else {
					// set the string terminating NULL byte on the end
					// of the data read
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}
}
