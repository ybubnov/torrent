#pragma once

#include <string>

namespace network{
	namespace bittorrent{
		namespace peer_wire{
			typedef class Peer{
				private:
					std::string _ip;
					std::string _port;

				public:
					Peer(std::string ip, std::string port);
					std::string ip();
					std::string port();
			}peer;
		}
	}
}
