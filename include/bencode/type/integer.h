#pragma once

#include "bencode/type/element.h"

/*format: i<integer encoded in base ten ASCII>e
 */

namespace bencode{
    namespace type{
        typedef class BencodeInteger : public BencodeElement{
            private:
                int64_t integer;

            public:
                BencodeInteger(int64_t integer);
                void* decode();
                std::vector<char> bencode();
                static int64_t decode(BencodeElement* decoded);
        }integer;
    }
}
