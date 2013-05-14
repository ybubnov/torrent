#include "TrackerProtocol.h"

const char TrackerProtocol::question_mark = '?';
const int TrackerProtocol::active_thread = 0;
const int TrackerProtocol::finish_thread = -1;

TrackerProtocol::TrackerProtocol(TorrentFileProvider file) : torrentFile(file){

	encryption::random_string_generator generator;
	key = generator.ascii_string(8);

	std::vector<char> appendix = generator.random_string(12);
	peer_id = "-UT3300-";
	raw_peer_id.insert(raw_peer_id.end(), peer_id.begin(), peer_id.end());
	raw_peer_id.insert(raw_peer_id.end(), appendix.begin(), appendix.end());

	peer_id += encryption::percent_encoding::encode(appendix);
}

TrackerProtocol::~TrackerProtocol(){
	delete trackerResponse;
}

void TrackerProtocol::yeild(){
	trackerRequest = new boost::thread(boost::bind(&TrackerProtocol::announce_request, this));
	trackerRequest->yield();
}

void TrackerProtocol::execute_downloading(){
	std::cout << "EXECUTE" << std::endl;
	std::cout << "PEER " << peerList.size() << std::endl;

	if(peerList.size() == 0){
		return;
	}

	pieceControl = new PieceControl(torrentFile.pieces(), torrentFile.piece_length());

	for(std::list<network::bittorrent::peer_wire::peer>::iterator it = peerList.begin();
		it != peerList.end(); it++){


		network::bittorrent::peer_wire::protocol* peerWireProtocol = 
			new network::bittorrent::peer_wire::protocol((*it), pieceControl, downloadFile, info_hash, raw_peer_id);

		peerWireProtocolList.push_back(peerWireProtocol);

		//(*peerWireProtocolList.begin())->yeild();
		peerWireProtocol->yeild();

	}
}


void TrackerProtocol::response_handle(std::istream* response){
	std::cout << "response handle" << std::endl;
	std::string item;
	responseData.clear();

	threadStack.pop();

	network::http::parser hparser(response);									//parse to HTTP response format
	hparser.parse();

	responseData = hparser.http_data();		

	if(hparser.bad()){															//normal HTTP response status: {"HTTP 1.1", "200", "OK"}
		std::cout << "HTTP BAD" << std::endl;
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

	}catch(std::exception e){
		std::cout << e.what();
	}

	std::cout << "peer list " << peerList.size() << std::endl;
	std::cout << "active " << threadStack.size() << std::endl;

	int nextAction = threadStack.top();

	if(nextAction == TrackerProtocol::finish_thread){
		peerConversation = new boost::thread(boost::bind(&TrackerProtocol::execute_downloading, this));
		peerConversation->yield();
	}
}

void TrackerProtocol::announce_request(){
	std::list<std::string> announce = torrentFile.announce_list();
	std::string httpDomain;
	std::string httpUri;
	std::string httpRequest;

	/*fileList = torrentFile.files();
	DownloadFile dFile = *fileList.begin();
	downloadFile = new FinalFile("e:\\" + dFile.path, dFile.length, torrentFile.piece_length());*/


	threadStack.push(finish_thread);
	
	for(unsigned int i = 0; i < torrentFile.announce_size(); i++){
		httpList.push_back(new network::http::protocol(this));
		threadStack.push(active_thread);
	}

	std::cout << "STACK SIZE " << threadStack.size() << std::endl;

	std::list<network::http::protocol*>::iterator httpItem = httpList.begin();
	info_hash = torrentFile.info_hash();

	std::string percent_info_hash = encryption::percent_encoding::encode(info_hash);

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
			<< "&numwant=20"
			<< "&compact=1"
			<< "&no_peer_id=1";

		std::cout << request.str() << std::endl << std::endl;

		httpRequest = request.str();
		http->get(httpDomain, httpRequest);
	}
}