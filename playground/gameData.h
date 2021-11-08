#pragma once
#ifndef GAME_VIEW_H
#define GAME_VIEW_H

//#include <memory>

#include <glfw3.h>

#include "gameLogic.h"

using namespace gameLogic;

namespace gameData {


	const int boardX = 7, boardY = 7;

	const float oneSquareX = 2.0f / boardX;
	const float oneSquareY = 1.7f / boardY;

	extern std::shared_ptr<Board> gameBoard;

	const float spaceNorm = 0.04f, spaceInnerPlate = 0.1f, spaceFirstLine = 0.2f;


	static GLfloat rect_vertex_buffer[3 * 6]{
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	static GLfloat coin_vertex_buffer[3 * 6]{
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		/*0.0f, 0.0f, 0.0f,
		0.0f, 0.33f, 0.0f,
		0.33f, 0.0f, 0.0f,
		0.0f, 0.66f, 0.0f,
		0.0f, 0.1f, 0.0f,
		0.33f, 0.1f, 0.0f,*/

	};

	static GLfloat W_Char_back[3 * 3 * 2]{
		0.0f,	0.0f,	0.0f,
		0.3f,	1.0f,	0.0f,
		0.6f,	0.0f,	0.0f,
		0.4f,	0.0f,	0.0f,
		0.7f,	1.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
	};

	static GLfloat W_Char_front[3 * 3 * 2]{
		0.14f,	0.0f,	0.0f,
		0.3f,	0.55f,	0.0f,
		0.46f,	0.0f,	0.0f,
		0.54f,	0.0f,	0.0f,
		0.7f,	0.55f,	0.0f,
		0.86f,	0.0f,	0.0f,
	};

	static GLfloat I_Char[3 * 3 * 2]{
		0.4f,	0.0f,	0.0f,
		0.4f,	1.0f,	0.0f,
		0.6f,	0.0f,	0.0f,
		0.6f,	0.0f,	0.0f,
		0.4f,	1.0f,	0.0f,
		0.6f,	1.0f,	0.0f,
	};

	static GLfloat N_Char[3 * 3 * 3]{
		0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,
		1.0f,	1.0f,	0.0f,
		0.8f,	0.0f,	0.0f,
		0.8f,	1.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		0.8f,	1.0f,	0.0f,
		1.0f,	1.0f,	0.0f,
	};

	static GLfloat N_Char_front[9]{
		0.2f,	0.5f,	0.0f,
		0.2f,	1.0f,	0.0f,
		0.7f,	1.0f,	0.0f,
	};

	static GLfloat S_Char[3 * 3 * 4]{
		0.5f,	0.0f,	0.0f,
		0.25f,	0.5f,	0.0f,
		0.5f,	0.5f,	0.0f,
		0.5f,	0.0f,	0.0f,
		0.5f,	0.5f,	0.0f,
		0.75f,	0.0f,	0.0f,
		0.5f,	0.5f,	0.0f,
		0.25f,	1.0f,	0.0f,
		0.5f,	1.0f,	0.0f,
		0.5f,	0.5f,	0.0f,
		0.5f,	1.0f,	0.0f,
		0.75f,	0.5f,	0.0f,
	};


	constexpr int NOTHING = 0, RED = 3, BLUE = 6, BACKGROUND = 9, GREY = 12;


	static float colors[3 * 5] = {
		0.5f,	0.5f,	0.5f,
		0.95f,	0.0f,	0.0f,
		0.0f,	0.0f,	0.95f,
		0.1f,	0.1f,	0.1f,
		0.7f,	0.7f,	0.7f
	};

	static GLfloat g_vertex_buffer_data[3 * 6 * 7 * 6 * 2 + 7 * 6 * 3 + 16 * 3 * 3];
	static GLfloat g_color_buffer_data[3 * 6 * 7 * 6 * 2 + 7 * 6 * 3 + 16 * 3 * 3];

	void initBoard();
}

#endif

