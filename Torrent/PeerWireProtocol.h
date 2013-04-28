#pragma once

#include "network.h"
#include "PieceControl.h"

namespace network{
	namespace bittorrent{
		namespace peer_wire{
			typedef class PeerWireProtocol : public network::responsible{
				private:
					std::vector<char> _info_hash;
					std::vector<char> _peer_id;

					peer_wire::peer _peer;
					message::type _last_message;
					boost::thread _conversation;

					PieceControl* _piece_control;
					network::tcp::protocol* _tcp;

					void conversation();
				public:
					PeerWireProtocol(
						peer_wire::peer stranger, 
						PieceControl* piece_control, 
						std::vector<char> info_hash, 
						std::vector<char> peer_id
					);

					PeerWireProtocol(
						peer_wire::peer stranger, 
						PieceControl* piece_control, 
						std::string info_hash, 
						std::string peer_id
					);

					~PeerWireProtocol();

					void response_handle(std::istream* response);
					void yeild();
			}protocol;
		}
	}
}