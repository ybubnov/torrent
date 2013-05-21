#include "HttpParser.h"

using namespace network::http;

HttpParser::HttpParser(std::istream& provider){
	char buf[0x7fff];

	while(!provider.eof()){
		provider.read(buf, sizeof(buf));
		sbuf.sputn(buf, provider.gcount());
	}

	this->provider = new std::istream(&sbuf);
}

HttpParser::~HttpParser(){
	delete provider;
}

void HttpParser::parse(){
	std::string item;
	std::stringstream stream;
	long read_size = 0;
	char buffer[0xffff];

	memset(buffer, 0, sizeof(buffer));
	provider->getline(buffer, sizeof(buffer), '\n');
	stream << buffer;

	for(int i = 0; i < 3; i++){
		stream >> item;
		statusLine.push_back(item);
	}

	std::string key;
	std::string value;

	do{
		memset(buffer, 0, sizeof(buffer));
		provider->getline(buffer, sizeof(buffer), '\n');
		std::stringstream flow;
		flow << buffer;

		if(buffer[0] == 13){
			continue;
		}

		flow >> key;
		value = std::string(flow.str());
		value = value.substr(value.find(key) + key.size() + 1);

		if((key.at(key.size() - 1) == ':')){
			httpHeaders.insert(std::pair<std::string, std::string>(key, value));
		}else{
			memset(buffer, 0, sizeof(buffer));
			provider->read(buffer, sizeof(buffer));

			httpData = std::vector<char>(buffer, buffer + provider->gcount() - 2);
			break;
		}
	}while(!provider->eof());
}

std::vector<std::string> HttpParser::status_line(){
	return statusLine;
}

std::map<std::string, std::string> HttpParser::http_headers(){
	return httpHeaders;
}

std::vector<char> HttpParser::http_data(){
	return httpData;
}

bool HttpParser::bad(){
	if(!statusLine.size()){
		throw std::bad_exception();
	}

	return statusLine[1].compare("200") != 0;
}
