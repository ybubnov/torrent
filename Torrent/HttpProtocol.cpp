#include "HttpProtocol.h"

using namespace network::http;

HttpProtocol::HttpProtocol(network::responsible* listener_object) : socket(io_service){
	listener = listener_object;
	response = &network::responsible::response_handle;
}

HttpProtocol::~HttpProtocol(){
	socket.close();
	io_service.stop();
}

void HttpProtocol::read_handler(const boost::system::error_code& ec, std::size_t bytes_transferred){
	response(listener, new std::istream(&http_response));
}

void HttpProtocol::get(std::string host, std::string uri){

	io_service.reset();
	boost::asio::streambuf http_request;
	std::ostream request_stream(&http_request);

	request_stream << "GET " << uri << " HTTP/1.1\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Accept: text/html\r\n";
	request_stream << "Connection: close\r\n\r\n";

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
