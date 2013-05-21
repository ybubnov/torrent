#pragma once



#include "FinalFile.h"
#include "NotifiableInterface.h"

namespace network{
	namespace bittorrent{
		namespace io{
			typedef class LoadAdapter{
				private:
					std::vector<FinalFile*> _file_list;
					std::vector<long> _block_list;
					std::vector<int64_t> _length_list;

					int64_t _loaded;
					int64_t _total_length;

					long _piece_length;
					network::bittorrent::io::notifiable* _subscriber;
					boost::mutex _guard;
					//boost::mutex _left_guard;

				public:
					LoadAdapter(network::bittorrent::io::notifiable* subscriber, std::list<FinalFile*>& file_list, long piece_length);
					~LoadAdapter();

					int64_t left();
					int64_t downloaded();
					void write(std::vector<char>& data, long piece_number);
			}load_adapter;
		}
	}
}
