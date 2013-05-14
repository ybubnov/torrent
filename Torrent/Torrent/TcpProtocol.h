#pragma once

#include <algorithm>
#include <ctime>
#include <iostream>
#include <ostream>
#include <string>
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include <boost\thread.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "ResponsibleInterface.h"

namespace network{
	namespace tcp{
		typedef class TcpProtocol : private boost::noncopyable{
			private:
				network::responsible* get_callback;
				network::responsible* send_callback;

				std::string address;
				std::string port;

				boost::thread thread;
				boost::asio::io_service io_service;
				boost::asio::ip::tcp::acceptor _acceptor;
				boost::asio::io_service::strand _strand;
				boost::asio::ip::tcp::socket _socket;
				boost::array<char, 65536> _tcp_response;
		

				void get_handle(const boost::system::error_code& error);
				void send_handle(const boost::system::error_code& error);

			public:
				TcpProtocol(const std::string& address, const std::string& port);

				void get(network::responsible* callback);
				void send(network::responsible* callback, std::vector<char> buffer);
				void send(std::vector<char> buffer);

				void stop();
		}protocol;
	}
}