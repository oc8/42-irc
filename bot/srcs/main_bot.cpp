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
vector<string> split(string str, string delimiter)
{
	if (delimiter == "")
		return vector<string>(1, str);
	size_t pos;
	vector<string> tokens = vector<string>();
	while ((pos = str.find(delimiter)) != string::npos)
	{
		if (pos > 0)
			tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	if (tokens.size() == 0 || str != "")
		tokens.push_back(str);
	return tokens;
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
	while (42)
	{
		botch.read();
		std::vector<string> tokens = ::split(botch.get_msg(), "\n");
		for (size_t i = 0; i < tokens.size(); ++i) {
			botch.pars(tokens[i]);
			botch.reply();
		}
	}
	return 0;
}
