#include "server.hpp"

Server::Server(int port, string password)
{
	this->port = port;
	this->password = password;
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);

}

Server::~Server() {}

void Server::socket() {
	master_socket = socket(
}

void Server::setsockopt() {

}

void Server::bind() {

}

void Server::listen() {

}

void Server::accept() {

}

void Server::read() {

}

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
	if (::bind(this->master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
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
