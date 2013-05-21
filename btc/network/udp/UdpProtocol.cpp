#include "UdpProtocol.h"

using namespace network::udp;

/*

UdpProtocol::UdpProtocol(network::responsible* listener_object) : socket(io_service){
	listener = listener_object;
	response = &network::responsible::response_handle;
}

UdpProtocol::~UdpProtocol(){
	io_service.stop();
}

void UdpProtocol::read_handler(const boost::system::error_code& ec, std::size_t bytes_transferred){
	//response(listener, new std::istream(&udp_response));
}

void UdpProtocol::get(std::string host, std::string port, std::string data){

	try{
		io_service.reset();

		boost::asio::ip::udp::resolver resolver(io_service);
		boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), host, port);
		sender_endpoint = *resolver.resolve(query);

		//socket.close();
		socket.open(boost::asio::ip::udp::v4());
		
		socket.send_to(boost::asio::buffer(data), sender_endpoint);

		socket.async_receive_from(
			boost::asio::buffer(udp_response),
			sender_endpoint,
			boost::bind(
				&UdpProtocol::read_handler, 
				this, 
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred
			));

		io_service.run();
		//std::cout.write(recv_buf.data(), len);

	}catch(std::exception e){
		std::cerr << e.what() << std::endl;
	}
}*/
