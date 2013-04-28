#pragma once

#include <string>
#include <strstream>
#include <vector>
#include <iomanip>

class PercentEncoding{
	public:
		static std::string encode(std::string source);
		static std::string encode(std::vector<char> source);
};
