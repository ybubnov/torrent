#pragma once

#include <vector>


namespace network{
	namespace bittorrent{
		namespace message{
			
			typedef enum{
				_cancel,
				_choke,
				_handshake,
				_have,
				_interested,
				_keep_alive,
				_not_interested,
				_request,
				_unchoke,
				_undefined
			}type;


			typedef class MessageInterface{
				public:
					virtual std::vector<char> message() = 0;
					virtual type what() = 0;
			}interface;
		}
	}
}