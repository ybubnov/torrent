#include "bencode.h"
#include "network.h"

#include "encryption.h"
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

#include "PeerWireProtocol.h"


class Temp : public network::responsible{
	public:
		void response_handle(std::istream* response){
			std::string string;
			std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
			while(!response->eof()){
				*response >> string;
				std::cout << string;
			}
			std::cout << "done" << std::endl;
			std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
		}
};

int main(){
	/*std::string str1 = "";
	std::string str2 = "";

	std::vector<char> v1(str1.begin(), str1.end());
	std::vector<char> v2(str2.begin(), str2.end());

	Temp t1;
	Temp t2;

	network::tcp::protocol tcp("telecom.by", "3000");

	tcp.send(&t1, v1);
	tcp.send(&t2, v2);*/


	/*char data[256] = {"hello, world"};
	std::vector<char> buffer(data, data + 13);

	network::tcp::protocol tcp("wikipedia.org", "80");
	tcp.send(buffer);*/

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

	/*Temp t;

	network::tcp::protocol tcp("192.168.0.100", "6881");
	//tcp.get(&t);
	boost::thread tr(boost::bind(&network::tcp::protocol::get, &tcp, &t));
	tr.yield();*/


	//std::ofstream log("log.txt", std::ios::trunc | std::ios::binary);
	//std::cout.rdbuf(log.rdbuf());

	//network::bittorrent::peer_wire::protocol p(

	bencode::provider::stream streamprovider("mmm.torrent");

	bencode::parser parser(streamprovider);
	bencode::element* result = parser.node();
	TorrentFileProvider tfile(result);

	std::string hash = tfile.info_hash();


	std::list<DownloadFile> fileList = tfile.files();
	DownloadFile dFile = *fileList.begin();
	FinalFile* downloadFile = new FinalFile("e:\\" + dFile.path, dFile.length, tfile.piece_length());

	/*system("pause");

	TrackerProtocol protocol(tfile);
	protocol.yeild();

	while(true){
		std::cout << "out" << std::endl;
		Sleep(500);
	}*/

	return 0;
}