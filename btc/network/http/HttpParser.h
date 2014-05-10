#pragma once

#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <cstring>

/*allows you to parse the http response
 */

namespace network{
    namespace http{
        typedef class HttpParser{
            private:
                std::stringbuf sbuf;
                std::istream* provider;
                std::vector<std::string> statusLine;
                std::map<std::string, std::string> httpHeaders;
                std::vector<char> httpData;

            public:
                HttpParser(std::istream& provider);
                ~HttpParser();
        
                void parse();
                std::vector<std::string> status_line();
                std::map<std::string, std::string> http_headers();
                std::vector<char> http_data();

                bool bad();
        }parser;
    }
}
