#pragma once

#include <string>
#include <vector>
#include <list>

/*provides an interface for a serial access to a source of bencoded data
 */

namespace bencode{
    namespace provider{
        typedef class BencodeProviderInterface{
            public:
                virtual std::vector<char> get() = 0;                                //get a symbol from a sequence
                virtual std::vector<char> get(int) = 0;                                //get couple of symbols from a sequnce
                virtual std::vector<char> peek() = 0;                                //return symbol from sequence (do not extract)
        }interface;
    }
}
