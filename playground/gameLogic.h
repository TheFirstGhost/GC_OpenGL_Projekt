#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <memory>




namespace gameLogic {

	class Board
	{
		const int xF, yF;
		
		std::shared_ptr<int[]> board;
	public:
		int throwField;
		Board(const int _xF, const int _yF);


		void setField(int x, int y, int color);

		const int getField(int x, int y);

		/*void setThrowField(int num) {
			throwField = num;
		}

		int getThrowField() {
			return throwField;
		}*/
	};

}

#endif
