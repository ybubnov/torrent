#include "TrackerProtocol.h"

const char TrackerProtocol::question_mark = '?';

TrackerProtocol::TrackerProtocol(TorrentFileProvider file) : 
	torrentFile(file),
	trackerRequest(boost::bind(&TrackerProtocol::announce_request, this)){

	RandomStringGenerator generator;

	key = generator.ascii_string(8);
	peer_id = "-UT3300-";
	peer_id += generator.ascii_string(12);
}

TrackerProtocol::~TrackerProtocol(){
	delete trackerResponse;
}

void TrackerProtocol::yeild(){
	trackerRequest.yield();
}

void TrackerProtocol::response_handle(std::istream* response){
	std::cout << "response handle" << std::endl;
	std::string item;
	responseData.clear();


	network::http::parser hparser(response);									//parse to HTTP response format
	hparser.parse();

	responseData = hparser.http_data();		

	if(hparser.bad()){															//normal HTTP response status: {"HTTP 1.1", "200", "OK"}
		return;
	}

	bencode::provider::vector provider(responseData);							//tracker response parse
	bencode::parser parser(provider);
	bencode::element* node;

	try{
		node = parser.node();
		trackerResponse = new TrackerResponse(node);							//tracker response wrapper

		if(trackerResponse->bad()){												//if tracker responce with errors
			throw std::exception();
		}

		std::vector<char> peers = trackerResponse->binary_peers();				//get peers in binary-style
		network::bittorrent::peer_wire::parser peerParser(peers);				//get 'normal' ipv4 addresses
		std::list<network::bittorrent::peer_wire::peer> additionalPeers = peerParser.peers();
		peerList.insert(peerList.end(), additionalPeers.begin(), additionalPeers.end());

		if(!additionalPeers.size()){											//if response without peer list
			return;
		}

		pieceControl = new PieceControl(torrentFile.pieces(), torrentFile.piece_length());
		peerWireProtocol = new network::bittorrent::peer_wire::protocol(*peerList.begin(), pieceControl, info_hash, peer_id);

		peerWireProtocol->yeild();
	}catch(std::exception e){
		std::cout << e.what();
	}
}

void TrackerProtocol::announce_request(){
	std::list<std::string> announce = torrentFile.announce_list();
	std::string httpDomain;
	std::string httpUri;
	std::string httpRequest;

	fileList = torrentFile.files();
	
	for(unsigned int i = 0; i < torrentFile.announce_size(); i++){
		httpList.push_back(new network::http::protocol(this));
	}

	std::list<network::http::protocol*>::iterator httpItem = httpList.begin();
	info_hash = torrentFile.info_hash();

	std::string percent_info_hash = PercentEncoding::encode(info_hash);

	for(std::list<std::string>::iterator it = announce.begin(); it != announce.end(); it++, httpItem++){
		std::string httpAddress = *it;
		network::http::protocol* http = *httpItem;

		std::stringstream request;

		httpDomain = network::http::address_parser::domain(httpAddress);
		httpUri = network::http::address_parser::uri(httpAddress);
		
		request << "/" << httpUri;

		if(httpUri.find(question_mark) != std::string::npos){
			request << "&info_hash=";
		}else{
			request << "?info_hash=";
		}

		request << percent_info_hash
			<< "&peer_id=" << peer_id
			<< "&port=" << "6881"
			<< "&uploaded=0"
			<< "&downloaded=0"
			<< "&left=" << torrentFile.size()
			<< "&corrupt=0"
			<< "&key=" << key
			<< "&event=started"
			<< "&numwant=50"
			<< "&compact=1"
			<< "&no_peer_id=1";

		std::cout << request.str() << std::endl << std::endl;

		httpRequest = request.str();
		http->get(httpDomain, httpRequest);

		std::cout << "http" << std::endl;

		//break;						// <------ NEED CORRECTION ???
	}
}