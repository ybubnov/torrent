#pragma once

#include "MessageInterface.h"

namespace network{
	namespace bittorrent{
		namespace message{
			typedef class Choke : public MessageInterface{
				private:
					std::vector<char> data;
					Choke();

				public:
					static Choke create();
					std::vector<char> message();
					type what();
			}choke;
		}
	}
}