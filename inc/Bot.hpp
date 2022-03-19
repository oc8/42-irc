#pragma once

#include "User.hpp"
#include "Channel.hpp"

class Bot : public User
{
public:
	Bot(){};
	Bot(std::string name);
	~Bot();

	void hello(Channel &chan);
	void welcome(Channel &chan, User &user);
};
