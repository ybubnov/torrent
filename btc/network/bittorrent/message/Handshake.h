#pragma once

#include <string>
#include "MessageInterface.h"

namespace network{
	namespace bittorrent{
		namespace message{
			typedef class Handshake : public MessageInterface{
				private:
					std::vector<char> data;

					Handshake(std::vector<char> info_hash, std::vector<char> peer_id);

				public:
					static Handshake create(std::string info_hash, std::string peer_id);
					static Handshake create(std::vector<char> info_hash, std::vector<char> peer_id);
					
					static bool is(std::vector<char>& input);
                    static std::vector<char> crop(std::vector<char>& incoming);     //cut message from the incoming

					std::vector<char> message();
					type what();
			}handshake;
		}
	}
}
