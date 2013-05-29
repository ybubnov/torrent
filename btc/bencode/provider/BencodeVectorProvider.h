#pragma once

#include <vector>
#include <algorithm>
#include "BencodeProviderInterface.h"

/*allows you to get a serial access to a bencoded data vector
 */

namespace bencode{
	namespace provider{
		typedef class BencodeVectorProvider : public BencodeProviderInterface{
			private:
				std::vector<char> source;
				long index;

			public:
				BencodeVectorProvider(std::vector<char> source);
				~BencodeVectorProvider();

				std::vector<char> get();
				std::vector<char> get(int length);
				std::vector<char> peek();
		}vector;
	}
}
