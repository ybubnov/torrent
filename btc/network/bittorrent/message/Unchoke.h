#pragma once

#include "MessageInterface.h"

namespace network{
    namespace bittorrent{
        namespace message{
            typedef class Unchoke : public MessageInterface{
                private:
                    std::vector<char> data;
                    Unchoke();

                public:
                    static Unchoke create();
                    std::vector<char> message();

                    static bool is(std::vector<char>& input);
                    type what();
            }unchoke;
        }
    }
}
