#pragma once

#include <sstream>
#include "BencodeConsumerInterface.h"

namespace bencode{
	namespace consumer{
		typedef class BencodeStringConsumer : BencodeConsumerInterface{
			private:
				std::stringstream destination;

			public:
				BencodeStringConsumer();
				~BencodeStringConsumer();

				void set(bencode::element* encoded);
				void dispose();
				std::vector<char> vector();
		}string;
	}
}