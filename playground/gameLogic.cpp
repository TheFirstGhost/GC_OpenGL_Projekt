#include "gameLogic.h"
//#include "gameData.h"

using namespace gameLogic;

namespace gameLogic{
	Board::Board(const int _xF, const int _yF) : xF(_xF), yF(_yF) {
		board = std::shared_ptr<int[]>{ new int[xF * yF], std::default_delete<int[]>() };
		throwField = 0;
	}

	void Board::setField(int x, int y, const int color) {
		board[x + xF * y] = color;
	}

	const int Board::getField(int x, int y) {
		return board[x + xF * y];
	}
}

