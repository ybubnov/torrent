#include "network.h"
#include "encryption.h"


class Temp : public network::bittorrent::io::notifiable{
	public:
		void notify(double percents){
			std::cout << percents << "%%" << std::endl;
		}
};

int main(){
	Temp t;

	bencode::provider::stream streamprovider("17.torrent");

	bencode::parser parser(streamprovider);
	bencode::element* result = parser.node();
	network::bittorrent::file_parser tfile(result);

	system("pause");

	network::bittorrent::protocol protocol(&t, tfile, L"e:\\");
	protocol.yeild();

	while(true){
		std::cout << "out" << std::endl;
		Sleep(500);
	}

	return 0;
}