#include "PieceControl.h"

PieceControl::PieceControl(std::vector<char> pieces, size_t piece_length){
	_pieces = pieces;
	_piece_length = piece_length;

	int piece_count = _pieces.size() / 20;
	std::vector<char> piece;
	std::vector<char>::iterator first;
	std::vector<char>::iterator last;

	for(int i = 0; i < piece_count; i++){
		first = pieces.begin() + i * 20;
		last = first + 20;

		piece = std::vector<char>(first, last);

		_piece_map.insert(std::pair<int, std::vector<char>>(i, piece));
	}
}

void PieceControl::downloaded(Piece piece){
	_piece_map.erase(piece.index());
}

int PieceControl::piece_length(){
	return _piece_length;
}

Piece PieceControl::next_piece(){
	int index = _piece_map.begin()->first;
	std::vector<char> piece = _piece_map.begin()->second;

	return Piece::create(piece, index);
}