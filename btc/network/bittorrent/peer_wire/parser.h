#pragma once

#include <vector>
#include <list>
#include <sstream>
#include <iomanip>

#include "peer.h"

namespace network{
    namespace bittorrent{
        namespace peer_wire{
            class parser{
                private:
                    std::vector<char> _peers_vector;
                    std::list<peer> _peers_list;

                    int num(char value);

                public:
                    parser(std::vector<char> peers);
                    std::list<peer> peers();
            };
        }
    }
}
