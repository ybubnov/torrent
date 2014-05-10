#pragma once

#include <vector>
#include <cmath>
#include "BlockProviderInterface.h"

namespace encryption{
    namespace sha{
        namespace provider{
            typedef class BlockVectorProvider : public BlockProviderInterface{
                private:
                    int length;
                    std::list<long> bytelist;

                public:
                    BlockVectorProvider(std::vector<char> source);
                    std::list<long> block();
            }vector;
        }
    }
}
