#pragma once

#include "MessageInterface.h"

namespace network{
    namespace bittorrent{
        namespace message{
            typedef class Request : public MessageInterface{
                private:
                    std::vector<char> data;
                    Request(long index, long begin, long length);

                public:
                    static Request create(long index, long begin, long length);
                    std::vector<char> message();
                    type what();
            }request;
        }
    }
}
