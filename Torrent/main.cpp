#include "bencode.h"
#include "network.h"

#include "SecureHashAlgorithm.h"
#include "BlockStringProvider.h"
#include "TorrentFileProvider.h"
#include "TrackerProtocol.h"
#include "TrackerResponse.h"
#include "PeerParser.h"
#include "PieceControl.h"

#include "Handshake.h"
#include "Choke.h"
#include "Unchoke.h"
#include "Interested.h"
#include "NotInterested.h"
#include "Request.h"


class Temp : public network::responsible{
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

int main(){
	/*Temp t;

	network::tcp::protocol _tcp("127.0.0.1", "17100");
	network::bittorrent::message::interface& handshake = network::bittorrent::message::handshake::create("9d2031f14363000fffd17511de2b01babf2f2881", "-UT3310-123456789123");

	_tcp.send(&t, handshake.message());*/

	/*Temp t;

	network::tcp::protocol tcp("192.168.0.100", "3000");
	tcp.get(&t);

	while(true){
		std::cout << "out" << std::endl;
		Sleep(500);
	}*/

	/*bencode::provider::stream streamprovider("announce.php");
	bencode::parser parser(streamprovider);
	bencode::element* node = parser.node();

	TrackerResponse response(node);

	std::string _failure_reason = response.failure_reason();
	std::string _warning_message = response.warning_message();
	std::vector<char> _tracker_id = response.tracker_id();
	std::vector<char> _peers = response.binary_peers();
	long _interval = response.interval();
	long _min_interval = response.min_interval();
	long _complete = response.complete();
	long _incomplete = response.incomplete();

	PeerParser p(_peers);
	std::list<Peer> peers = p.peers();*/

	bencode::provider::stream streamprovider("b.torrent");

	bencode::parser parser(streamprovider);
	bencode::element* result = parser.node();
	TorrentFileProvider tfile(result);

	/*std::vector<char> pieces = tfile.pieces();
	size_t length = tfile.piece_length();

	PieceControl(pieces, length);*/


	TrackerProtocol protocol(tfile);
	protocol.yeild();

	while(true){
		std::cout << "out" << std::endl;
		Sleep(1000);
	}
	
	system("pause");



	/*Temp t1;
	network::tcp::protocol tcp1("31.43.162.198", "62970");
	Handshake handshake = Handshake::create("847bc2f6c478fb4e93b6531738f12c60fb3c59a0", "2d5554333130302d2f682f9eee0be0136a94731f");

	tcp1.send(&t1, handshake.message());
	*/
	system("pause");

	return 0;
}