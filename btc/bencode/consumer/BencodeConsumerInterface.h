#pragma once

#include <fstream>
#include "../type.h"

/*provides an interface for entities that
 *encapsulate the bencoded data storage
 */

namespace bencode{
	namespace consumer{
		typedef class BencodeConsumerInterface{
			public:
                virtual void set(element*) = 0;                     //append new bencoded element
				virtual void dispose() = 0;
                std::vector<char> vector();                         //get data as byte sequence
		}interface;
	}
}
