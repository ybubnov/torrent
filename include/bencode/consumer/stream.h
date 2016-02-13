#if 0

#pragma once

#include <cstring>
#include <boost/filesystem/fstream.hpp>
#include "BencodeConsumerInterface.h"

/*allows you to write bencoded data to a file
 */

namespace bencode{
    namespace consumer{
        typedef class BencodeStreamConsumer : public BencodeConsumerInterface{
            private:
                std::wstring _path;
                boost::filesystem::ofstream destination;

            public:
                BencodeStreamConsumer(std::wstring path);
                ~BencodeStreamConsumer();

                void set(bencode::element* encoded);
                void dispose();
                std::vector<char> vector();
        }stream;
    }
}

#endif
