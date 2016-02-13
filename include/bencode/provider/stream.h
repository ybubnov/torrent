#if 0

#ifndef INCLUDE_bencode_provider__
#define INCLUDE_bencode_provider__

#include <fstream>
#include <algorithm>
#include <cstring>
#include <boost/filesystem/fstream.hpp>
#include "BencodeProviderInterface.h"

/*allows you a serial access to bencoded data file
 */

namespace bencode{
namespace provider{

typedef class BencodeStreamProvider : public BencodeProviderInterface {
private:
    boost::filesystem::ifstream source;

public:
    BencodeStreamProvider(std::string path);
    BencodeStreamProvider(std::wstring path);
    ~BencodeStreamProvider();

    std::vector<char> get();
    std::vector<char> get(int length);
    std::vector<char> peek();
} stream;

}
}

#endif // INCLUDE_bencode_provider__

#endif
