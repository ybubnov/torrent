#include "PieceControl.h"

using namespace network::bittorrent::piece;

PieceControl::PieceControl(std::vector<char> pieces, size_t piece_length){
	_pieces = pieces;
	_piece_length = piece_length;

    _piece_count = _pieces.size() / 20;
	std::vector<char> piece;
	std::vector<char>::iterator first;
	std::vector<char>::iterator last;

    for(long i = 0; i < _piece_count; i++){
		first = pieces.begin() + i * 20;
		last = first + 20;

		piece = std::vector<char>(first, last);

		_piece_map.insert(std::pair<int, std::vector<char> >(i, piece));
		_shadow_map.push_back(std::pair<long, std::vector<char> >(i, piece));
	}

	_next_piece = _shadow_map.begin();
}

PieceControl::~PieceControl(){
}

void PieceControl::downloaded(TorrentPiece* piece){
	boost::mutex::scoped_lock lock(_guard);

	_piece_map.erase(piece->index());
}

long PieceControl::piece_length(){
	return _piece_length;
}

long PieceControl::left(){
	boost::mutex::scoped_lock lock(_guard);

	return _piece_map.size();
}

long PieceControl::count(){
    return _piece_count;
}

TorrentPiece* PieceControl::next_piece(){
	boost::mutex::scoped_lock lock(_guard);

	int index;
	std::vector<char> piece;


	if(_next_piece != _shadow_map.end()){
		index = _next_piece->first;
		piece = _next_piece->second;

		_next_piece++;
	}else{
		_shadow_map.clear();
		_shadow_map = std::list<std::pair<long, std::vector<char> > >(_piece_map.begin(), _piece_map.end());

		if(_shadow_map.size()){
			_next_piece = _shadow_map.begin();

			index = _next_piece->first;
			piece = _next_piece->second;
			_next_piece++;
		}else{
            std::cout << "THERE IS NO MORE PIECES TO LOAD" << std::endl;
			throw std::exception();
		}
	}

	return TorrentPiece::create(piece, index);

}
