#pragma once

#include <string>
#include <vector>

namespace encryption{
	namespace codecvt{
		class wstring_convert{
			public:
				static std::wstring codecvt_utf8_utf16(std::vector<char> data);
				static std::string codecvt_utf16_utf8(std::wstring source);
		};
	}
}
