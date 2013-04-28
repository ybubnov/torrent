#include "TcpProtocol.h"

using namespace network::tcp;

TcpProtocol::TcpProtocol(const std::string& address, const std::string& port) :  
	_acceptor(io_service),
	_strand(io_service),
	_socket(io_service){

	this->address = address;
	this->port = port;
}

void TcpProtocol::get(network::responsible* callback){
	get_callback = callback;
	io_service.reset();

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	_acceptor.open(endpoint.protocol());
	_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	_acceptor.bind(endpoint);
	_acceptor.listen();

	_acceptor.async_accept(
		_socket,
		boost::bind(
			&TcpProtocol::get_handle, 
			this,
			boost::asio::placeholders::error
			)
		);

	io_service.run();
}

void TcpProtocol::send(network::responsible* callback, std::vector<char> buffer){
	send_callback = callback;
	io_service.reset();

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	std::cout << endpoint;

	_socket.close();
	_socket.connect(endpoint);

	boost::asio::write(_socket, boost::asio::buffer(buffer));

	_socket.async_send(
		boost::asio::buffer(buffer),
		boost::bind(
			&TcpProtocol::send_handle,
			this,
			boost::asio::placeholders::error
			)
		);

	io_service.run();
}

void TcpProtocol::send_handle(const boost::system::error_code& error){
	io_service.stop();
	send_callback->response_handle(0);
}

void TcpProtocol::get_handle(const boost::system::error_code& error){
	io_service.stop();
	size_t bytes_transfered;
	bytes_transfered = _socket.read_some(boost::asio::buffer(_tcp_response));
	
	boost::asio::streambuf response;
	response.sputn(_tcp_response.data(), bytes_transfered);


	get_callback->response_handle(new std::istream(&response));
}

void TcpProtocol::stop(){
	io_service.stop();
}