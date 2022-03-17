#pragma once

#include "Server.hpp"

class Bot {
	public:
		Bot(string name);
		~Bot();

		void hello(string chan);
		void welcome(string chan);

	private:
		string name;
};
