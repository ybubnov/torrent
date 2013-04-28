#pragma once

#include "MessageInterface.h"

namespace network{
	namespace bittorrent{
		namespace message{
			typedef class Cancel : public MessageInterface{
				private:
					std::vector<char> data;
					Cancel(long index, long begin, long length);

				public:
					static Cancel create(long index, long begin, long length);
					std::vector<char> message();
					type what();
			}cancel;
		}
	}
}