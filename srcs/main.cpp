#include "server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Port or password not found" << endl;
		return 1;
	}
	int port = atoi(argv[1]);
	if (port < 1) {
		cout << "Invalid port" << endl;
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
