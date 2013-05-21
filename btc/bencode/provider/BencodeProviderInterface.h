#pragma once

#include <string>
#include <vector>
#include <list>

/*classes derived interface 'BenCodeProviderInterface' provide sequential access to bencode source*/

namespace bencode{
	namespace provider{
		typedef class BencodeProviderInterface{
			public:
				virtual std::vector<char> get() = 0;								//get one symbol from sequence
				virtual std::vector<char> get(int) = 0;								//get couple of symbols from sequnce
				virtual std::vector<char> peek() = 0;								//return symbol from sequence (do not extract)
		}interface;
	}
}