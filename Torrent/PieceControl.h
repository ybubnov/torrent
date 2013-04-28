#pragma once

#include <vector>
#include <map>

#include "Piece.h"

class PieceControl{
	private:
		std::vector<char> _pieces;
		size_t _piece_length;

		std::map<int, std::vector<char> > _piece_map;

	public:
		PieceControl(std::vector<char> pieces, size_t piece_length);
		
		void downloaded(Piece piece);
		int piece_length();
		Piece next_piece();
};