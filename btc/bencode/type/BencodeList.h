#pragma once

#include "BencodeElement.h"

/*format: <bencoded values>e
 */

namespace bencode{
    namespace type{
        typedef class BencodeList : public BencodeElement{
            private:
                bencode::list list;

            public:
                BencodeList(bencode::list list);
                ~BencodeList();

                void* decode();
                std::vector<char> bencode();
                static bencode::list decode(BencodeElement* decoded);
        }list;
    }
}
