#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Bot.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        cerr << "<ip> <port> <password>" << endl;
        return argc;
    }
    Bot botch("Botch", argv[1], atoi(argv[2]), argv[3]);
    if (botch < 1)
    {
        cerr << "Invalid port" << endl;
        return 1;
    }
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        cerr << "Invalid address or address not supported" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr << "Connection Failed" << endl;
        return -1;
    }
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
    return 0;
}
