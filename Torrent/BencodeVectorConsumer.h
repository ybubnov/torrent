#pragma once

#include <sstream>
#include "BencodeConsumerInterface.h"

namespace bencode{
	namespace consumer{
		typedef class BencodeVectorConsumer : BencodeConsumerInterface{
			private:
				std::vector<char> destination;

			public:
				BencodeVectorConsumer();

				void set(bencode::element* encoded);
				void dispose();
				std::vector<char> vector();
		}vector;
	}
}