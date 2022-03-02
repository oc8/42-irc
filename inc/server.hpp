#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	  //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0
#define PORT 8888

using std::cout;
using std::cerr;
using std::endl;
using std::string;

class Server {
public:
	Server(int port, string password);
	virtual ~Server();

	void socket();
	void setsockopt();
	void bind();
	void listen();
	void accept();
	void read();

private:
	int port;
	string password;
	int opt;
	int master_socket;
	int addrlen;
	int new_socket;
	struct sockaddr_in address;
};
