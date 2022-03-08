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
