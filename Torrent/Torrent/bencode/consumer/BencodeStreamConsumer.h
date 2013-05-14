#pragma once
#include "BencodeConsumerInterface.h"

namespace bencode{
	namespace consumer{
		typedef class BencodeStreamConsumer : BencodeConsumerInterface{
			private:
				std::string path;
				std::ofstream destination;

			public:
				BencodeStreamConsumer(std::string path);
				~BencodeStreamConsumer();

				void set(bencode::element* encoded);
				void dispose();
				std::vector<char> vector();
		}stream;
	}
}