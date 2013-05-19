#pragma once

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include <boost\thread.hpp>

#include "..\ResponsibleInterface.h"

namespace network{
	namespace udp{
		typedef class UdpProtocol{
			private:
				/*network::responsible* get_callback;
				network::responsible* send_callback;

				std::string address;
				std::string port;

				boost::thread thread;
				boost::asio::io_service io_service;
				boost::asio::ip::udp:: _acceptor;
				boost::asio::io_service::strand _strand;
				boost::asio::ip::tcp::socket _socket;
				boost::array<char, 65536> _tcp_response;
		

				void get_handle(const boost::system::error_code& error);
				void send_handle(const boost::system::error_code& error);*/

			public:
				/*UdpProtocol(const std::string& address, const std::string& port);

				void get(network::responsible* callback);
				void send(network::responsible* callback, std::vector<char>& buffer);

				void stop();*/
		}protocol;
	}
}