#include "HttpAddressParser.h"

using namespace network::http;

std::string HttpAddressParser::domain(std::string httpAddress){
    std::string httpDomain = httpAddress.substr(7);
    httpDomain = httpDomain.substr(0, httpDomain.find("/"));

    return httpDomain;
}

std::string HttpAddressParser::uri(std::string httpAddress){
    std::string httpUri = httpAddress.substr(7);
    httpUri = httpUri.substr(httpUri.find("/") + 1);

    return httpUri;
}
