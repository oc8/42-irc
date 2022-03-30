#include "Bot.hpp"

int ft_atoi(const char *str)
{
	int i = 0;
	int nb = 0;
	int sign = 1;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}


int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        cerr << "<ip> <port> <password>" << endl;
        return argc;
    }
    int port = ft_atoi(argv[2]);
    if (port < 1)
    {
        cerr << "Invalid port" << endl;
        return 1;
    }
    string ip = argv[1];
    string password = argv[3];
    Bot botch("Botch");
    botch.socket();
	botch.connect(ip, port);
	botch.send("PASS " + password);
	botch.send("NICK " + botch.get_nickname());
	botch.send("USER " + botch.get_nickname() + " 0 * :" + botch.get_nickname());
	botch.send("JOIN #bot");
	botch.send("PRIVMSG #bot:Le bot" + botch.get_nickname() + " est pret !");
	while (42) {
		botch.read();
		botch.pars();
		botch.reply();
	}
    return 0;
}
