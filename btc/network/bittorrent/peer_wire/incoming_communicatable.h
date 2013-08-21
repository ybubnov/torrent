#pragma once

namespace network{
    namespace bittorrent{
        namespace peer_wire{
            class incoming_communicatable{
                public:
                    virtual bool request() = 0;                                     //sended request message to the peer
                    virtual bool handshake() = 0;                                   //sended handshake message to the peer
                    virtual bool interested() = 0;                                  //sended interested message to the peer
                    virtual bool connect() = 0;                                     //established connection to peer address
                    virtual bool disconnect() = 0;

                    //etc... there is no time to implement full protocol

                    virtual ~incoming_communicatable(){}
            };
        }
    }
}
