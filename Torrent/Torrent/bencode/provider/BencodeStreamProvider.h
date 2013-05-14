#pragma once

#include <fstream>
#include <algorithm>
#include "BencodeProviderInterface.h"

namespace bencode{
	namespace provider{
		typedef class BencodeStreamProvider : public BencodeProviderInterface{
			private:
				std::ifstream source;

			public:
				BencodeStreamProvider(std::string path);
				~BencodeStreamProvider();

				std::vector<char> get();
				std::vector<char> get(int length);
				std::vector<char> peek();
		}stream;
	}
}