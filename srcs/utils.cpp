#include <vector>
#include <string>
#include <iostream>

using std::string;

std::vector<string> split(string str, string delimiter) {
	if (delimiter == "")
		return std::vector<string>(1, str);
	size_t pos;
	std::vector<string> tokens;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		if (pos > 0)
			tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	tokens.push_back(str);
	return tokens;
}
