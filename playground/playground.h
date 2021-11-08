#pragma once
#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>

//some global variables for handling the vertex buffer
GLuint vertexbuffer;
GLuint colorbuffer;
GLuint uvbuffer;
GLuint VertexArrayID;
GLuint vertexbuffer_size;

GLuint Texture;
GLuint TextureID;

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;

//contans
const int
	GAME_STATE_START = 0,
	GAME_STATE_PLAYER1 = 1,
	GAME_STATE_PLAYER2 = 2,
	GAME_STATE_END = 3;
int gameState = GAME_STATE_START;

int main( void ); //<<< main function, called at startup
void changeThrowInPos(int direction); //<<< move the coin in different positions
int throwIn(); //<<< write the last x-pos to the 
void setNextState(int state); //<<< change the Gamestate
bool checkWin(); //check if the current Player has won
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
void initVertexbuffer(); //<<< first init
void drawWin(int color); //<<< show the winning-sign
void clearWin(); //<<< reset the winning-sign
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW


#endif
