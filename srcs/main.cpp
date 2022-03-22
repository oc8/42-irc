#include "Server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cerr << "<port> <password>" << endl;
		return argc;
	}
	int port = atoi(argv[1]);
	if (port < 1)
	{
		cerr << "Invalid port" << endl;
		return 1;
	}
	string password = argv[2];
	Server server(port, password);
	server.socket();
	server.setsockopt();
	server.bind();
	server.listen();
	cout << "Waiting for connections..." << endl;
	server.read();
	return 0;
}
