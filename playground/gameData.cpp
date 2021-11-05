#include "gameData.h"


using namespace gameData;


namespace gameData {
	std::shared_ptr<Board> gameBoard = std::make_shared<Board>(boardX, boardY);


	void initBoard() {

		for (int x = 0; x < boardX; x++) {
			for (int y = 0; y < (boardY - 1); y++) {
				gameBoard->setField(x, y, NOTHING);
			}
		}

		for (int x = 0; x < boardX; x++)
		{
				gameBoard->setField(x, 6, BACKGROUND);
		}

	}
}


