#pragma once

#include <string>
#include <algorithm>

class Utf8Encoder{
	public:
		static std::string encode(std::wstring source);
};