#pragma once

#include "BlockProviderInterface.h"

namespace encryption{
    namespace sha{
        typedef class SecureHashAlgorithm{
            private:
                provider::interface* source;
                std::list<long> hashes;
                long leftrotate(long value, long n);

            public:
                SecureHashAlgorithm(provider::interface* source);
                std::string hash();                                                 //returns sha1 hash
                std::string url_hash();                                             //returns percent encoding of the sha1 hash
        }algorithm;
    }
}
