#pragma once

#include "MessageInterface.h"

namespace network{
    namespace bittorrent{
        namespace message{
            typedef class Have : public MessageInterface{
                private:
                    std::vector<char> data;
                    Have(long index);

                public:
                    static Have create(long index);
                    std::vector<char> message();
                    type what();

                    static std::vector<long> have(std::vector<char> incoming);
            }have;
        }
    }
}
