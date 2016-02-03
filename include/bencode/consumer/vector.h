#pragma once

#include <sstream>
#include "BencodeConsumerInterface.h"

/*allows you to write bencoded data to a vector
 */

namespace bencode{
    namespace consumer{
        typedef class BencodeVectorConsumer : public BencodeConsumerInterface{
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
