#pragma once

#include <string>
#include <sstream>
#include <list>
#include <iomanip>

class BlockProviderInterface{
	public:
		virtual std::list<long> block() = 0;
};