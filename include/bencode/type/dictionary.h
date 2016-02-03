#pragma once

#include "BencodeElement.h"

/*format: d<bencoded string><bencoded element>e
 */

namespace bencode{
    namespace type{
        typedef class BencodeDictionary : public BencodeElement{
            private:
                bencode::map dictionary;

            public:
                BencodeDictionary(bencode::map dictionary);
                ~BencodeDictionary();

                void* decode();
                std::vector<char> bencode();
                static bencode::map decode(BencodeElement* decoded);
        }dictionary;
    }
}
