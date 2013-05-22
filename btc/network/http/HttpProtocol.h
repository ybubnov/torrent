#pragma once

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "../ResponsibleInterface.h"

namespace network{
	namespace http{
		typedef class HttpProtocol{
			private:
				boost::asio::streambuf http_response;
				boost::asio::io_service io_service;
				boost::asio::ip::tcp::socket socket;
				network::responsible* listener;


				void read_handler(const boost::system::error_code& ec, std::size_t bytes_transferred);

			public:
				HttpProtocol(network::responsible* listener_object);
				~HttpProtocol();
				void get(std::string host, std::string uri);
		}protocol;
	}
}
