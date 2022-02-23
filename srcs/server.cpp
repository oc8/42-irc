#include <iostream>
#include <string>
#include <stdlib.h>
// #include <socket>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
	if (argc != 3)
		cout << "Port or password not found" << endl;
	int port = atoi(argv[1]);
	if (port < 1)
		cout << "Invalid port" << endl;
	string password = argv[2];
	cout << port << endl;
	cout << password << endl;
   // struct sockaddr_in address;
	// address.sin_family = AF_INET;
   // address.sin_addr.s_addr = INADDR_ANY;
   // address.sin_port = htons( PORT );
	return 0;
}
