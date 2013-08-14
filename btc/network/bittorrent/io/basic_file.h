#pragma once

#include <vector>

typedef long long int64_t;

namespace network{
    namespace bittorrent{
        namespace io{
            class basic_file{
                public:
                    virtual int64_t length() = 0;
                    virtual int64_t left() = 0;
                    virtual int64_t downloaded() = 0;

                    virtual void write(std::vector<char>&, int64_t) = 0;

                    virtual ~basic_file(){}
            };
        }
    }
}
