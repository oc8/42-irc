#include <vector>
#include <string>
#include <iostream>

using std::string;

int ft_atoi(string str)
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

std::vector<string> split(string str, string delimiter)
{
	if (delimiter == "")
		return std::vector<string>(1, str);
	size_t pos;
	std::vector<string> tokens;
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		if (pos > 0)
			tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	if (tokens.size() == 0 || str != "")
		tokens.push_back(str);
	return tokens;
}

string erase_str_in_str(string str, string erase)
{
	size_t pos = 0;
	while ((pos = str.find(erase, pos)) != std::string::npos) {
		str.erase(pos, erase.length());
	}
	return str;
}
