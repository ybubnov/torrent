#pragma once

#include "TorrentFileProvider.h"
#include "TrackerResponse.h"
#include "FinalFile.h"
#include "PeerWireProtocol.h"

#include "encryption.h"
#include "bencode.h"
#include "network.h"

#include <vector>
#include <sstream>
#include <list>
#include <stack>

class TrackerProtocol : protected network::responsible{
	private:
		static const char question_mark;							//const value of question mark letter
		static const int active_thread;
		static const int finish_thread;

		std::list<network::http::protocol*> httpList;				//list of announce http addresses
		std::vector<char> responseData;								//http-style response on http protocol announce request
		std::list<network::bittorrent::peer_wire::peer> peerList;	//list of active peers
		std::list<DownloadFile> fileList;							//list of files in torrent file


		TorrentFileProvider torrentFile;							//torrent file wrapper
		TrackerResponse* trackerResponse;							//torrent tracker responce wrapper

		FinalFile* downloadFile;									//real data file

		boost::thread* trackerRequest;								//thread for tracker requests
		boost::thread* peerConversation;							//thread for conversations with peers

		std::string info_hash;
		std::string peer_id;
		std::vector<char> raw_peer_id;
		std::string key;											//announe request field

		PieceControl* pieceControl;
		std::list<network::bittorrent::peer_wire::protocol*> peerWireProtocolList;

		std::stack<int> threadStack;

	protected:
		void announce_request();
		void execute_downloading();

	public:
		TrackerProtocol(TorrentFileProvider file);
		~TrackerProtocol();

		void response_handle(std::istream* response);
		void yeild();	
};