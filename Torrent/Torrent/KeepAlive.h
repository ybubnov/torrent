#pragma once

#include "MessageInterface.h"

namespace network{
	namespace bittorrent{
		namespace message{
			typedef class KeepAlive : public MessageInterface{
				private:
					std::vector<char> data;
					KeepAlive();

				public:
					static KeepAlive create();
					std::vector<char> message();
					type what();
			}keep_alive;
		}
	}
}