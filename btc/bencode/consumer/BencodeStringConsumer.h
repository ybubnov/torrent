#pragma once

#include <sstream>
#include "BencodeConsumerInterface.h"

/*allow to write bencoded data to a string
 */

namespace bencode{
	namespace consumer{
        typedef class BencodeStringConsumer : public BencodeConsumerInterface{
			private:
                std::string destination;

			public:
				BencodeStringConsumer();
				~BencodeStringConsumer();

				void set(bencode::element* encoded);
				void dispose();
				std::vector<char> vector();
		}string;
	}
}
