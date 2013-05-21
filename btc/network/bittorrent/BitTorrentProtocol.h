#pragma once

#include "TorrentFileParser.h"
#include "TrackerResponseParser.h"

#include "io.h"
#include "peer_wire.h"
#include "..\http.h"

#include <vector>
#include <sstream>
#include <list>
#include <stack>

namespace network{
	namespace bittorrent{
		typedef class BitTorrentProtocol : public network::responsible{
			private:
				static const char question_mark;								//const value of question mark letter
				static const int active_thread;
				static const int finish_thread;

				std::list<network::http::protocol*> httpList;					//list of announce http addresses
				std::vector<char> responseData;									//http-style response on http protocol announce request
				std::list<network::bittorrent::peer_wire::peer> peerList;		//list of active peers
				std::list<DownloadFile> fileList;								//list of files in torrent file
				std::list<network::bittorrent::io::file*> finalFiles;


				network::bittorrent::file_parser torrentFile;			//torrent file wrapper
				network::bittorrent::tracker_response_parser* trackerResponse;	//torrent tracker responce wrapper
				network::bittorrent::io::notifiable* _subscriber;

				network::bittorrent::io::load_adapter* loadAdapter;

				boost::thread* trackerRequest;									//thread for tracker requests
				boost::thread* peerConversation;								//thread for conversations with peers
				boost::mutex _guard;

				long _active_threads;

				std::string info_hash;
				std::wstring download_folder;
				std::string peer_id;
				std::vector<char> raw_peer_id;
				std::string key;												//announe request field

				network::bittorrent::piece::control* pieceControl;
				std::list<network::bittorrent::peer_wire::protocol*> peerWireProtocolList;

				std::stack<int> threadStack;

			protected:
				void announce_request();
				void execute_downloading();

			public:
				BitTorrentProtocol(network::bittorrent::io::notifiable* subscriber, network::bittorrent::file_parser file, std::wstring folder);
				~BitTorrentProtocol();

				void response_handle(std::istream& response);
                void interrupt();
				void yeild();	
		}protocol;
	}
}
