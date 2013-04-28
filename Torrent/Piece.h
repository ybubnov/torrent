#pragma once

#include <vector>

class Piece{
	private:
		std::vector<char> _piece;
		int _index;

		Piece(std::vector<char> piece, int index);

	public:
		static Piece create(std::vector<char>, int index);

		int index();
		std::vector<char> piece();
};