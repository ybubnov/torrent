#pragma once

#include <fstream>
#include <algorithm>
#include <cstring>
#include <boost/filesystem/fstream.hpp>
#include "BencodeProviderInterface.h"

namespace bencode{
	namespace provider{
		typedef class BencodeStreamProvider : public BencodeProviderInterface{
			private:
                boost::filesystem::ifstream source;

			public:
				BencodeStreamProvider(std::string path);
                BencodeStreamProvider(std::wstring path);
				~BencodeStreamProvider();

				std::vector<char> get();
				std::vector<char> get(int length);
				std::vector<char> peek();
		}stream;
	}
}
