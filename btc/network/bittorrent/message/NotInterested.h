#pragma once

#include "MessageInterface.h"

namespace network{
    namespace bittorrent{
        namespace message{
            typedef class NotInterested : public MessageInterface{
                private:
                    std::vector<char> data;
                    NotInterested();

                public:
                    static NotInterested create();
                    std::vector<char> message();
                    type what();
            }not_interested;
        }
    }
}
