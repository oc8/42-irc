#include <vector>
#include <string>
#include <iostream>

using std::string;

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
