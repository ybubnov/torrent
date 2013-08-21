#pragma once

#include "incoming_communicatable.h"
#include "../piece.h"
#include "../io.h"
#include "../message.h"
#include "../peer_wire.h"
#include "../../tcp.h"

/*bittorrent peer wire
 */

namespace network{
    namespace bittorrent{
        namespace peer_wire{
            class incoming_protocol : public network::responsible, public incoming_communicatable{
                private:
                    const static long block_length;

                    std::vector<char> _info_hash;
                    std::vector<char> _peer_id;
                    std::vector<char> _incoming;
                    std::vector<bool> _have;
                    std::vector<char> _buffer;

                    network::bittorrent::peer_wire::peer _peer;
                    network::bittorrent::message::type _last_message;

                    network::bittorrent::piece::control* _piece_control;
                    network::bittorrent::piece::torrent_piece* _current_piece;
                    network::bittorrent::io::basic_file* _file;

                    network::tcp::protocol* _tcp;
                    boost::thread* _conversation;                                   //thread descriptor

                    long _piece_length;
                    long _begin;
                    long _offset;

                    void initialize();

                    bool refresh();                                                 //recovering
                    void shift();                                                   //piece more than _block_length

                public:
                    incoming_protocol(
                        network::bittorrent::peer_wire::peer stranger,
                        network::bittorrent::piece::control* piece_control,
                        network::bittorrent::io::basic_file* file,
                        std::vector<char> info_hash,
                        std::vector<char> peer_id
                    );

                    incoming_protocol(
                        network::bittorrent::peer_wire::peer stranger,
                        network::bittorrent::piece::control* piece_control,
                        io::basic_file* file,
                        std::string info_hash,
                        std::string peer_id
                    );

                    incoming_protocol(
                        network::bittorrent::peer_wire::peer stranger,
                        network::bittorrent::piece::control* piece_control,
                        network::bittorrent::io::basic_file* file,
                        std::string info_hash,
                        std::vector<char> peer_id
                    );

                    ~incoming_protocol();

                    bool request();
                    bool handshake();
                    bool interested();
                    bool connect();
                    bool disconnect();

                    void response_handle(std::istream& response);
            };
        }
    }
}
