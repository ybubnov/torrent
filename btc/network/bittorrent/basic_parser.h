#pragma once

namespace network{
    namespace bittorrent{
        class file_stat{
            public:
                int64_t length;
                std::wstring path;
                std::string md5sum;

                file_stat(int64_t length, std::wstring path, std::string md5sum = ""){
                    this->length = length;
                    this->path = path;
                    this->md5sum = md5sum;
                }
        };

        class basic_parser{
            public:
                virtual ~basic_parser(){}

                virtual std::wstring created_by(){throw std::exception();}
                virtual std::wstring encoding(){throw std::exception();}

                virtual std::wstring name() = 0;
                virtual std::wstring comment() = 0;
                virtual std::string creation_date() = 0;

                virtual std::list<file_stat> files() = 0;
                virtual std::string size() = 0;
                virtual std::string info_hash() = 0;
                virtual std::list<std::string> announce_list() = 0;
                virtual std::vector<char> pieces() = 0;

                virtual size_t piece_length() = 0;
        };
    }
}

