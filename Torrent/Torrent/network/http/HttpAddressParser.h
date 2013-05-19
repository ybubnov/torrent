#pragma once

#include <string>
#include <sstream>

namespace network{
	namespace http{
		typedef class HttpAddressParser{
			public:
				static std::string domain(std::string httpAddress);
				static std::string uri(std::string httpAddress);
		}address_parser;
	}
}
