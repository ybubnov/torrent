#pragma once


#include "../piece.h"
#include "../io.h"
#include "../message.h"
#include "../peer_wire.h"
#include "../../tcp.h"


namespace network{
	namespace bittorrent{
		namespace peer_wire{
			typedef class PeerWireProtocol : public network::responsible{
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
					network::bittorrent::io::load_adapter* _file;
					
                    network::game_overable* _dad;
					network::tcp::protocol* _tcp;
					boost::thread* _conversation;
					
					long _piece_length;
					long _begin;
					long _offset;

					void initialize();
					void conversation();
					


					bool request();

					bool handshake();
					bool interested();

					bool refresh();
					void shift();

				public:
					PeerWireProtocol(
                        network::game_overable* dad,
						network::bittorrent::peer_wire::peer stranger, 
						network::bittorrent::piece::control* piece_control,
						network::bittorrent::io::load_adapter* file,
						std::vector<char> info_hash, 
						std::vector<char> peer_id
					);

					PeerWireProtocol(
                        network::game_overable* dad,
						network::bittorrent::peer_wire::peer stranger, 
						network::bittorrent::piece::control* piece_control, 
						io::load_adapter* file,
						std::string info_hash, 
						std::string peer_id
					);

					PeerWireProtocol(
                        network::game_overable* dad,
						network::bittorrent::peer_wire::peer stranger, 
						network::bittorrent::piece::control* piece_control,
						network::bittorrent::io::load_adapter* file,
						std::string info_hash, 
						std::vector<char> peer_id
					);

					~PeerWireProtocol();

					void response_handle(std::istream& response);
                    void interrupt();
					void yeild();

			}protocol;
		}
	}
}
