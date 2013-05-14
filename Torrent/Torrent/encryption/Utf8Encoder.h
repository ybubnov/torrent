#pragma once

#include <string>
#include <algorithm>

namespace encryption{
	typedef class Utf8Encoder{
		public:
			static std::string encode(std::wstring source);
	}utf8_encoder;
}
