#include "TcpProtocol.h"

using namespace network::tcp;

TcpProtocol::TcpProtocol(const std::string& address, const std::string& port) :  
	_acceptor(io_service),
	_strand(io_service),
	_socket(io_service){

	this->address = address;
	this->port = port;

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	std::cout << endpoint << std::endl;
	_socket.connect(endpoint);
	io_service.run();
}

void TcpProtocol::get(network::responsible* callback){
	get_callback = callback;

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

void TcpProtocol::send(std::vector<char> buffer){
	boost::asio::write(_socket, boost::asio::buffer(buffer));
}

void TcpProtocol::send(network::responsible* callback, std::vector<char> buffer, long bytes_to_read){
	send_callback = callback;
	boost::asio::write(_socket, boost::asio::buffer(buffer));

	send_handle(bytes_to_read, boost::system::error_code());
}

void TcpProtocol::send(network::responsible* callback, std::vector<char> buffer){
	send_callback = callback;
	boost::asio::write(_socket, boost::asio::buffer(buffer));

	/*_socket.async_send(
		boost::asio::buffer(buffer),
		boost::bind(
			&TcpProtocol::send_handle,
			this,
			boost::asio::placeholders::error
			)
		);*/

	send_handle(boost::system::error_code());
}

void TcpProtocol::send_handle(long bytes_to_read, const boost::system::error_code& error){
	boost::asio::streambuf response;

	try{
		boost::asio::read(_socket, boost::asio::buffer(_tcp_response), boost::asio::transfer_exactly(bytes_to_read));
	}catch(std::exception e){
		_socket.read_some(boost::asio::buffer(_tcp_response));

	}

	response.sputn(_tcp_response.data(), bytes_to_read);

	std::istream response_stream(&response);
	std::istream& response_lnk = response_stream;

	send_callback->response_handle(response_lnk);
}

void TcpProtocol::send_handle(const boost::system::error_code& error){
	boost::asio::deadline_timer timer(io_service);
	boost::asio::streambuf response;
	size_t bytes_transfered;
	size_t available;

	available = _socket.available();

	do{
		timer.expires_from_now(boost::posix_time::milliseconds(400));
		timer.wait();

		bytes_transfered = _socket.read_some(boost::asio::buffer(_tcp_response));
		response.sputn(_tcp_response.data(), bytes_transfered);
		available = bytes_transfered;

		timer.expires_from_now(boost::posix_time::milliseconds(400));
		timer.wait();

		available = _socket.available();
	}while(available);

	std::istream response_stream(&response);
	std::istream& response_lnk = response_stream;

	send_callback->response_handle(response_lnk);
}

void TcpProtocol::get_handle(const boost::system::error_code& error){
	size_t bytes_transfered;
	bytes_transfered = _socket.read_some(boost::asio::buffer(_tcp_response));
	
	boost::asio::streambuf response;
	response.sputn(_tcp_response.data(), bytes_transfered);

	std::istream response_stream(&response);
	std::istream& response_lnk = response_stream;

	get_callback->response_handle(response_lnk);
}

void TcpProtocol::stop(){
	io_service.stop();
}
