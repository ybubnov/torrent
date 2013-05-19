#pragma once

#include <fstream>
#include "..\type.h"

namespace bencode{
	namespace consumer{
		typedef class BencodeConsumerInterface{
			public:
				virtual void set(element*) = 0;
				virtual void dispose() = 0;
				std::vector<char> vector();
		}interface;
	}
}