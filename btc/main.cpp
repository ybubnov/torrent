#include "network.h"
#include "interface.h"
//#include "encryption.h"


class Temp : public network::bittorrent::io::notifiable{
	public:
		void notify(double percents){
			std::cout << percents << " %" << std::endl;
		}
};

int main(int argc, char** argv){
    QApplication a(argc, argv);

    interface::window w;
    w.show();

    /*Temp t;

    bencode::provider::stream streamprovider("C:\\Users\\brick\\Downloads\\8.torrent");

	bencode::parser parser(streamprovider);
	bencode::element* result = parser.node();
	network::bittorrent::file_parser tfile(result);

    system("pause");

	network::bittorrent::protocol protocol(&t, tfile, L"e:\\");
	protocol.yeild();

	while(true){
		//std::cout << "out" << std::endl;
		Sleep(1000);
    }*/

    return a.exec();
}
