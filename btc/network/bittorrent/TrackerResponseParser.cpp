#include "TrackerResponseParser.h"

using namespace network::bittorrent;

TrackerResponseParser::TrackerResponseParser(bencode::element* dictionary){
    source = bencode::type::dictionary::decode(dictionary);

    std::vector<char> key;
    std::string value;

    _complete = 0;
    _incomplete = 0;
    _min_interval = 0;
    _interval = 0;
    
    _fr = false;
    _wm = false;
    _ti = false;
    _bp = false;
    _i = false;
    _mi = false;
    _c = false;
    _ic = false;

    for(bencode::map::iterator it = source.begin(); it != source.end(); it++){
        key = bencode::type::string::decode(it->first);
        value = std::string(key.begin(), key.end());

        response.insert(std::pair<std::string, bencode::element*>(value, it->second));
    }

}

TrackerResponseParser::~TrackerResponseParser(){
    bencode::element::gc();
}

std::string TrackerResponseParser::failure_reason(){
    std::vector<char> key;

    if(_fr){
        return _failure_reason;
    }

    _fr = true;

    std::map<std::string, bencode::element*>::iterator failure_ptr = response.find("failure reason");

    if(failure_ptr != response.end()){
        key = bencode::type::string::decode(failure_ptr->second);
        _failure_reason = std::string(key.begin(), key.end());
    }

    return _failure_reason;
}

std::string TrackerResponseParser::warning_message(){
    std::vector<char> key;

    if(_wm){
        return _warning_message;
    }

    _wm = true;

    std::map<std::string, bencode::element*>::iterator warning_ptr = response.find("warning message");

    if(warning_ptr != response.end()){
        key = bencode::type::string::decode(warning_ptr->second);
        _warning_message = std::string(key.begin(), key.end());
    }

    return _warning_message;
}

std::vector<char> TrackerResponseParser::tracker_id(){
    if(_ti){
        return _tracker_id;
    }

    _ti = true;

    std::map<std::string, bencode::element*>::iterator id_ptr = response.find("tracker id");

    if(id_ptr != response.end()){
        _tracker_id = bencode::type::string::decode(id_ptr->second);
    }

    return _tracker_id;
}

std::vector<char> TrackerResponseParser::binary_peers(){
    if(_bp){
        return _bpeers;
    }

    _bp = true;


    std::map<std::string, bencode::element*>::iterator peers_ptr = response.find("peers");

    if(peers_ptr != response.end()){
        _bpeers = bencode::type::string::decode(peers_ptr->second);
    }

    return _bpeers;
}


long TrackerResponseParser::interval(){
    if(_i){
        return _interval;
    }

    _i = true;

    std::map<std::string, bencode::element*>::iterator interval_ptr = response.find("interval");

    if(interval_ptr != response.end()){
        _interval = (long)bencode::type::integer::decode(interval_ptr->second);
    }

    return _interval;
}

long TrackerResponseParser::min_interval(){
    if(_mi){
        return _min_interval;
    }

    _mi = true;

    std::map<std::string, bencode::element*>::iterator min_ptr = response.find("min interval");

    if(min_ptr != response.end()){
        _min_interval = (long)bencode::type::integer::decode(min_ptr->second);
    }

    return _min_interval;
}

long TrackerResponseParser::complete(){
    if(_c){
        return _complete;
    }

    _c = true;

    std::map<std::string, bencode::element*>::iterator complete_ptr = response.find("complete");

    if(complete_ptr != response.end()){
        _complete = (long)bencode::type::integer::decode(complete_ptr->second);
    }

    return _complete;
}

long TrackerResponseParser::incomplete(){
    std::vector<char> key;
    std::string value;

    if(_ic){
        return _incomplete;
    }

    _ic = true;

    std::map<std::string, bencode::element*>::iterator incomplete_ptr = response.find("incomplete");

    if(incomplete_ptr != response.end()){
        _incomplete = (long)bencode::type::integer::decode(incomplete_ptr->second);
    }

    return _incomplete;
}


bool TrackerResponseParser::bad(){
    return (warning_message().size() && failure_reason().size());
}
