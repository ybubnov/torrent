#include "HttpProtocol.h"

using namespace network::http;

HttpProtocol::HttpProtocol(network::responsible* listener_object) : socket(io_service){
	listener = listener_object;
}

HttpProtocol::~HttpProtocol(){
	socket.close();
	io_service.stop();
}

void HttpProtocol::read_handler(const boost::system::error_code& ec, std::size_t bytes_transferred){
	std::istream response_stream(&http_response);
	std::istream& response_lnk = response_stream;

	listener->response_handle(response_lnk);
}

void HttpProtocol::get(std::string host, std::string uri){

	io_service.reset();
	boost::asio::streambuf http_request;
	std::ostream request_stream(&http_request);

	request_stream << "GET " << uri << " HTTP/1.1\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "User-Agent: uTorrent/3300\r\n";
	//request_stream << "Accept-Encoding: gzip\r\n";
	request_stream << "Connection: Close\r\n\r\n";

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(host, "http");
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	socket.close();
	socket.connect(*endpoint_iterator);

	boost::asio::write(socket, http_request);

	boost::asio::async_read(
		socket,
		http_response,
		boost::bind(
			&HttpProtocol::read_handler, 
			this, 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred
			)
		);


	/*boost::asio::async_read_until(
		socket, 
		http_response,
		"\r\n",
		boost::bind(
			&HttpProtocol::read_handler, 
			this, 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred
			)
		);*/


	boost::shared_ptr<boost::thread> thread(new boost::thread(
	boost::bind(&boost::asio::io_service::run, &io_service)));
	thread->yield();

	//io_service.run();
}
