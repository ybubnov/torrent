#pragma once

#include "TorrentFileProvider.h"
#include "RandomStringGenerator.h"
#include "TrackerResponse.h"
#include "FinalFile.h"
#include "PeerWireProtocol.h"

#include "bencode.h"
#include "network.h"

#include <vector>
#include <sstream>
#include <list>

class Response : public network::responsible{
	public:
		void response_handle(std::istream* response){
			/*HttpParser parser(response);
			parser.parse();*/

			std::stringstream string_flow;
			std::string string;
			while(!response->eof()){
				*response >> string;
				std::cout << string;
			}
			std::cout << "done" << std::endl;
		}
};


class TrackerProtocol : protected network::responsible{
	private:
		static const char question_mark;							//const value of question mark letter

		std::list<network::http::protocol*> httpList;				//list of announce http addresses
		std::vector<char> responseData;								//http-style response on http protocol announce request
		std::list<network::bittorrent::peer_wire::peer> peerList;	//list of active peers
		std::list<DownloadFile> fileList;							//list of files in torrent file


		TorrentFileProvider torrentFile;							//torrent file wrapper
		TrackerResponse* trackerResponse;							//torrent tracker responce wrapper

		FinalFile* downloadFile;									//real data file

		boost::thread trackerRequest;								//thread for tracker requests

		std::string info_hash;
		std::string peer_id;
		std::string key;											//announe request field

		PieceControl* pieceControl;
		network::bittorrent::peer_wire::protocol* peerWireProtocol;


		/*Response _handle;
		network::tcp::protocol* _tcp;*/

	protected:
		void announce_request();

	public:
		TrackerProtocol(TorrentFileProvider file);
		~TrackerProtocol();

		void response_handle(std::istream* response);
		void yeild();	
};