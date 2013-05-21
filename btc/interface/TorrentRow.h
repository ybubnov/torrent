#pragma once

#include <string>

namespace interface{
    namespace utils{
        typedef class TorrentRow{

            private:
                std::string _info_hash;
                int _status;
                std::string _file_name;
                std::string _file_size;

            public:
                TorrentRow(std::string info_hash, std::string file_name, std::string file_size, int status = 0);

                std::string get_hash();
                int get_status();
                std::string get_file_name();
                std::string get_file_size();
        }row;
    }
}

