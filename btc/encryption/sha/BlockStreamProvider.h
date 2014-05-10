#pragma once

#include <fstream>
#include <cstring>
#include <cmath>
#include "BlockProviderInterface.h"

namespace encryption{
    namespace sha{
        namespace provider{
            typedef class BlockStreamProvider : public BlockProviderInterface{
                private:
                    int length;
                    std::list<long> bytelist;

                public:
                    BlockStreamProvider(std::string path);
                    std::list<long> block();
            }stream;
        }
    }
}
