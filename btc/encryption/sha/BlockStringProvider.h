#pragma once

#include <cstring>
#include <cmath>
#include "BlockProviderInterface.h"


namespace encryption{
    namespace sha{
        namespace provider{
            typedef class BlockStringProvider : public BlockProviderInterface{
                private:
                    int length;
                    std::list<long> bytelist;

                public:
                    BlockStringProvider(std::string source);
                    std::list<long> block();
            }string;
        }
    }
}
