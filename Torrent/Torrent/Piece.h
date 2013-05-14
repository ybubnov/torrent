#pragma once

#include "MessageInterface.h"

namespace network{
	namespace bittorrent{
		namespace message{
			typedef class Piece : public MessageInterface{
				private:
					std::vector<char> data;
					Piece(long index, long begin, std::vector<char> bulk);

				public:
					static Piece create(long index, long begin, std::vector<char> bulk);
					std::vector<char> message();
					type what();

					static std::vector<char> block(std::vector<char> incoming);
			}piece;
		}
	}
}