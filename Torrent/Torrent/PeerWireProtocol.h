#pragma once

#include "network.h"
#include "PieceControl.h"
#include "FinalFile.h"

namespace network{
	namespace bittorrent{
		namespace peer_wire{
			typedef class PeerWireProtocol : public network::responsible{
				private:
					const static long block_length;

					std::vector<char> _info_hash;
					std::vector<char> _peer_id;
					std::vector<long> _have;

					peer_wire::peer _peer;
					message::type _last_message;
					boost::thread* _conversation;

					PieceControl* _piece_control;
					TorrentPiece* _current_piece;
					FinalFile* _file;
					
					network::tcp::protocol* _tcp;

					std::vector<char> _buffer;
					long _piece_length;
					long _begin;

					void initialize();
					void conversation();
					void shift();


					void request();
					bool handshake();
					bool interested();

				public:
					PeerWireProtocol(
						peer_wire::peer stranger, 
						PieceControl* piece_control,
						FinalFile* file,
						std::vector<char> info_hash, 
						std::vector<char> peer_id
					);

					PeerWireProtocol(
						peer_wire::peer stranger, 
						PieceControl* piece_control, 
						FinalFile* file,
						std::string info_hash, 
						std::string peer_id
					);

					PeerWireProtocol(
						peer_wire::peer stranger, 
						PieceControl* piece_control, 
						FinalFile* file,
						std::string info_hash, 
						std::vector<char> peer_id
					);

					~PeerWireProtocol();

					void response_handle(std::istream* response);
					void yeild();
			}protocol;
		}
	}
}