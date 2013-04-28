#pragma once

#include "MessageInterface.h"

namespace network{
	namespace bittorrent{
		namespace message{
			typedef class Interested : public MessageInterface{
				private:
					std::vector<char> data;
					Interested();

				public:
					static Interested create();
					std::vector<char> message();
					type what();
			}interested;
		}
	}
}