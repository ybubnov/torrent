#pragma once

#include "FinalFile.h"
#include "NotifiableInterface.h"

/*allows you to record pieces of the files in
 */

namespace network{
	namespace bittorrent{
		namespace io{
			typedef class LoadAdapter{
				private:
                    std::vector<FinalFile*> _file_list;                             //list of the downloadable files
                    std::vector<long> _block_list;                                  //blocks per file
                    std::vector<int64_t> _length_list;                              //bytes per file

					int64_t _loaded;
					int64_t _total_length;

					long _piece_length;
                    network::bittorrent::io::notifiable* _subscriber;               //listener of the downloading process state
                    boost::mutex _guard;                                            //thread safety

				public:
                    LoadAdapter(network::bittorrent::io::notifiable* subscriber,
                                std::list<FinalFile*>& file_list, long piece_length);

					~LoadAdapter();

					int64_t left();
					int64_t downloaded();
					void write(std::vector<char>& data, long piece_number);
			}load_adapter;
		}
	}
}
