#pragma once


#include "incoming_protocol.h"
#include "../core.h"
#include "../../tcp.h"

/*bittorrent peer wire
 */

namespace network{
    namespace bittorrent{
        namespace peer_wire{
            class incoming_runner : public core::executable{
                private:
                    incoming_communicatable* _incoming;
                    network::game_overable* _dad;                                   //object's parent
                    boost::thread* _this;                                           //thread descriptor

                    void initialize();
                    void invoke();

                public:
                    incoming_runner(
                        network::game_overable* dad,
                        network::bittorrent::peer_wire::peer stranger,
                        network::bittorrent::piece::control* piece_control,
                        network::bittorrent::io::basic_file* file,
                        std::vector<char> info_hash,
                        std::vector<char> peer_id
                    );

                    incoming_runner(
                        network::game_overable* dad,
                        network::bittorrent::peer_wire::peer stranger,
                        network::bittorrent::piece::control* piece_control,
                        io::basic_file* file,
                        std::string info_hash,
                        std::string peer_id
                    );

                    incoming_runner(
                        network::game_overable* dad,
                        network::bittorrent::peer_wire::peer stranger,
                        network::bittorrent::piece::control* piece_control,
                        network::bittorrent::io::basic_file* file,
                        std::string info_hash,
                        std::vector<char> peer_id
                    );

                    ~incoming_runner();

                    void interrupt();                                               //stop the thread
                    void timed_join(long milliseconds);
                    void yeild();                                                   //run the new thread

            };
        }
    }
}
