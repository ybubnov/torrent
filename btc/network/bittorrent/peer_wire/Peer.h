#pragma once

#include <string>

namespace network{
    namespace bittorrent{
        namespace peer_wire{
            class peer{
                private:
                    std::string _ip;
                    std::string _port;

                public:
                    peer(std::string ip, std::string port);
                    std::string ip();
                    std::string port();
            };
        }
    }
}
