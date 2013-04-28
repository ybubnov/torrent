#include "TrackerResponse.h"

TrackerResponse::TrackerResponse(bencode::element* dictionary){
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

TrackerResponse::~TrackerResponse(){
	bencode::element::gc();
}

std::string TrackerResponse::failure_reason(){
	std::vector<char> key;
	std::string value;

	if(_fr){
		return _failure_reason;
	}

	_fr = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("failure reason")){
			key = bencode::type::string::decode(it->second);
			_failure_reason = std::string(key.begin(), key.end());

			break;
		}
	}

	return _failure_reason;
}

std::string TrackerResponse::warning_message(){
	std::vector<char> key;
	std::string value;

	if(_wm){
		return _warning_message;
	}

	_wm = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("warning message")){
			key = bencode::type::string::decode(it->second);
			_warning_message = std::string(key.begin(), key.end());

			break;
		}
	}

	return _warning_message;
}

std::vector<char> TrackerResponse::tracker_id(){
	std::vector<char> key;
	std::string value;

	if(_ti){
		return _tracker_id;
	}

	_ti = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("tracker id")){
			_tracker_id = bencode::type::string::decode(it->second);
			break;
		}
	}

	return _tracker_id;
}

std::vector<char> TrackerResponse::binary_peers(){
	std::vector<char> key;
	std::string value;

	if(_bp){
		return _bpeers;
	}

	_bp = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("peers")){
			_bpeers = bencode::type::string::decode(it->second);
			break;
		}
	}

	return _bpeers;
}


long TrackerResponse::interval(){
	std::vector<char> key;
	std::string value;

	if(_i){
		return _interval;
	}

	_i = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("interval")){
			_interval = (long)bencode::type::integer::decode(it->second);
			break;
		}
	}

	return _interval;
}

long TrackerResponse::min_interval(){
	std::vector<char> key;
	std::string value;

	if(_mi){
		return _min_interval;
	}

	_mi = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("min interval")){
			_min_interval = (long)bencode::type::integer::decode(it->second);
			break;
		}
	}

	return _min_interval;
}

long TrackerResponse::complete(){
	std::vector<char> key;
	std::string value;

	if(_c){
		return _complete;
	}

	_c = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("complete")){
			_complete = (long)bencode::type::integer::decode(it->second);
			break;
		}
	}

	return _complete;
}

long TrackerResponse::incomplete(){
	std::vector<char> key;
	std::string value;

	if(_ic){
		return _incomplete;
	}

	_ic = true;

	for(std::map<std::string, bencode::element*>::iterator it = response.begin(); it != response.end(); it++){
		value = it->first;

		if(!value.compare("incomplete")){
			_incomplete = (long)bencode::type::integer::decode(it->second);
			break;
		}
	}

	return _incomplete;
}


bool TrackerResponse::bad(){
	return (warning_message().size() && failure_reason().size());
}
