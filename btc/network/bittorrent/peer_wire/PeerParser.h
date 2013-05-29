#pragma once

#include <vector>
#include <list>
#include <sstream>
#include <iomanip>

#include "Peer.h"

namespace network{
	namespace bittorrent{
		namespace peer_wire{
			typedef class PeerParser{
				private:
					std::vector<char> _peers_vector;
					std::list<Peer> _peers_list;

					int num(char value);

				public:
					PeerParser(std::vector<char> peers);
					std::list<Peer> peers();
			}parser;
		}
	}
}
