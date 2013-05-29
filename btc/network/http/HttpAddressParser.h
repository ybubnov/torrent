#pragma once

#include <string>
#include <sstream>

/*separates domain and uri of the link
 */

namespace network{
	namespace http{
		typedef class HttpAddressParser{
			public:
				static std::string domain(std::string httpAddress);
				static std::string uri(std::string httpAddress);
		}address_parser;
	}
}
