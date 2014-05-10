#pragma once

#include "file.h"
#include "notifiable.h"

/*allows you to record pieces of the files in
 */

namespace network{
    namespace bittorrent{
        namespace io{
            class file_splitter : public basic_file{
                private:
                    std::vector<basic_file*> _file_list;                             //list of the downloadable files
                    std::vector<long> _block_list;                                  //blocks per file
                    std::vector<int64_t> _length_list;                              //bytes per file

                    int64_t _loaded;
                    int64_t _total_length;

                    long _piece_length;
                    network::bittorrent::io::notifiable* _subscriber;               //listener of the downloading process state
                    boost::mutex _guard;                                            //thread safety

                public:
                    file_splitter(network::bittorrent::io::notifiable* subscriber,
                                std::list<basic_file*>& file_list, long piece_length);

                    ~file_splitter();

                    int64_t left();
                    int64_t downloaded();
                    int64_t length();

                    void write(std::vector<char>& data, long piece_number);
                    void write(std::vector<char>& data, int64_t piece_number);
            };
        }
    }
}
