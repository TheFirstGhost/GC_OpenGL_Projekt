#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
using namespace glm;

//Tncluds for Text
#include <common/shader.hpp>
//#include <common/texture.hpp>
//#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
//#include <common/text2D.hpp>

//Includ own header Files
#include "gameData.h"
#include "gameLogic.h"


using namespace std;

using namespace gameData;
using namespace gameLogic;

//int throwInField;

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Init the gameboard
  initBoard();
  int testField = 3;
  bool onlyOnce = true;

  //Initialize vertex buffer
  initVertexbuffer();
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//start animation loop until escape key is pressed
	do{
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);

        char text[256];
        if (gameState == GAME_STATE_START) {
            if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    setNextState(GAME_STATE_PLAYER1);
                }
            }
            else {
                onlyOnce = true;
            }
        }
        else if (gameState == GAME_STATE_PLAYER1){
            //gameBoard->throwField = 3;
            //gameBoard->setField(gameBoard->throwField, 6, RED);
            if (glfwGetKey(window, GLFW_KEY_LEFT)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    changeThrowInPos(-1);
                }
            }
            else if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    changeThrowInPos(1);
                }
            }
            else if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    //gameState = GAME_STATE_PLAYER2;
                    int yPos = throwIn();
                    if (yPos != -1) {
                        gameBoard->setField(gameBoard->throwField, yPos, RED);
                        gameBoard->setField(gameBoard->throwField, 6, BACKGROUND);
                        setNextState(GAME_STATE_PLAYER2);
                        if (checkWin()) {
                            drawWin(RED);
                            setNextState(GAME_STATE_END);
                        }
                    }
                    
                }
            }
            else {
                onlyOnce = true;
            }

        }
        else if (gameState == GAME_STATE_PLAYER2)
        {
            //gameBoard->throwField = 3;
            //gameBoard->setField(gameBoard->throwField, 6, BLUE);
            if (glfwGetKey(window, GLFW_KEY_A)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    changeThrowInPos(-1);
                }
            }
            else if (glfwGetKey(window, GLFW_KEY_D)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    changeThrowInPos(1);
                }
            }
            else if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    //gameState = GAME_STATE_PLAYER1;
                    int yPos = throwIn();
                    if (yPos != -1) {
                        gameBoard->setField(gameBoard->throwField, yPos, BLUE);
                        gameBoard->setField(gameBoard->throwField, 6, BACKGROUND);
                        setNextState(GAME_STATE_PLAYER1);
                        if (checkWin()) {
                            //sprintf(text, "Test123");
                            //glfwSetWindowTitle(window, tilteArr);
                            //printText2D(text, 100, 100, 60);
                            drawWin(BLUE);
                            setNextState(GAME_STATE_END);
                        }
                    }
                }
            }
            else {
                onlyOnce = true;
            }
        }
        else if (gameState == GAME_STATE_END) {
            if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                if (onlyOnce) {
                    onlyOnce = false;
                    setNextState(GAME_STATE_START);
                }
            }
            else {
                onlyOnce = true;
            }
        }
        initializeVertexbuffer();
        updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
    //Cleanup and close window
    cleanupVertexbuffer();
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);

	closeWindow();
  
	return 0;
}

void changeThrowInPos(int direction) {
    
    if (direction == 1) {
        gameBoard->setField(gameBoard->throwField, 6, BACKGROUND);
        gameBoard->throwField += 1;
        if (gameBoard->throwField > 6) gameBoard->throwField = 6;
    }
    else if (direction == -1) {
        gameBoard->setField(gameBoard->throwField, 6, BACKGROUND);
        gameBoard->throwField -= 1;
        if (gameBoard->throwField < 0) gameBoard->throwField = 0;
        
    }
    if (gameState == GAME_STATE_PLAYER1) {
        gameBoard->setField(gameBoard->throwField, 6, RED);
    }
    else if (gameState == GAME_STATE_PLAYER2) {
        gameBoard->setField(gameBoard->throwField, 6, BLUE);
    }
    
}

int throwIn() {
    int yPos = -1;
    for (int y = 0;  y < boardY-1;  y++)
    {
        if (gameBoard->getField(gameBoard->throwField, y) == NOTHING) {
            yPos = y;
            break;
        }
    }
    return yPos;
    //if (yPos != -1) {
        //gameBoard->setField(gameBoard->throwField, yPos, color);
        
    //}
}

void setNextState(int state) {
    if (state != gameState) {
        gameState = state;
        if (state == GAME_STATE_PLAYER1) {
            gameBoard->throwField = 3;
            gameBoard->setField(gameBoard->throwField, 6, RED);
        }
        else if (state == GAME_STATE_PLAYER2) {
            gameBoard->throwField = 3;
            gameBoard->setField(gameBoard->throwField, 6, BLUE);
        }
        else if (state == GAME_STATE_START) {
            clearWin();
            initBoard();
        }
    }
}

bool checkWin() {
    int connectedR = 0;
    int connectedB = 0;
    //bool win = false;
    for (int x = 0; x < boardX; x++)
    {
        connectedB = 0;
        connectedR = 0;
        for (int y = 0; y < boardY - 1; y++) 
        {
            if (gameBoard->getField(x, y) == RED) {
                connectedB = 0;
                connectedR++;
            }
            else if (gameBoard->getField(x, y) == BLUE) {
                connectedB++;
                connectedR = 0;
            }
            else {
                connectedB = 0;
                connectedR = 0;
            }
            if (connectedR == 4) {
                cout << "Player Red Wins1";
                return true;
                
            }
            if (connectedB == 4) {
                cout << "Player Blue Wins1";
                return true;
            }
        }
    }
    
    for (int y = 0; y < boardY - 1; y++)
    {
        connectedR = 0;
        connectedB = 0;
        for (int x = 0; x < boardX; x++)
        {
            if (gameBoard->getField(x, y) == RED) {
                connectedB = 0;
                connectedR++;
            }
            else if (gameBoard->getField(x, y) == BLUE) {
                connectedB++;
                connectedR = 0;
            }
            else {
                connectedB = 0;
                connectedR = 0;
            }
            if (connectedR == 4) {
                cout << "Player Red Wins2";
                return true;

            }
            if (connectedB == 4) {
                cout << "Player Blue Wins2";
                return true;
            }
        }
    }
    for (int x = 0; x < boardX-3; x++)
    {
        //int Sx = x;
        for (int y = 0; y < boardY - 4; y++)
        {
            connectedR = 0;
            connectedB = 0;
            for (int i = 0; i < 4; i++)
            {
                if (gameBoard->getField(x+i, y+i) == RED) {
                    connectedB = 0;
                    connectedR++;
                }
                else if (gameBoard->getField(x+i, y+i) == BLUE) {
                    connectedB++;
                    connectedR = 0;
                }
                else {
                    connectedB = 0;
                    connectedR = 0;
                }
                if (connectedR == 4) {
                    cout << "Player Red Wins3";
                    return true;

                }
                if (connectedB == 4) {
                    cout << "Player Blue Wins3";
                    return true;
                }
            }
        }
    }
    for (int x = boardX; x >= 3; x--)
    {
        for (int y = 0; y < boardY - 4; y++)
        {
            connectedR = 0;
            connectedB = 0;
            for (int i = 3; i >= 0; i--)
            {
                if (gameBoard->getField(x - i, y + i) == RED) {
                    connectedB = 0;
                    connectedR++;
                }
                else if (gameBoard->getField(x - i, y + i) == BLUE) {
                    connectedB++;
                    connectedR = 0;
                }
                else {
                    connectedB = 0;
                    connectedR = 0;
                }
                if (connectedR == 4) {
                    cout << "Player Red Wins4";
                    return true;

                }
                if (connectedB == 4) {
                    cout << "Player Blue Wins4";
                    return true;
                }
            }
        }
    }
    return false;
}

void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : colors
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(
      1,                  // attribute 1.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 1024, "connect four", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  GLclampf red = colors[BACKGROUND];
  GLclampf green = colors[BACKGROUND + 1];
  GLclampf blue = colors[BACKGROUND + 2];

  // Dark grey background
  glClearColor(red, green, blue, 0.0f);

  return true;
}

void initVertexbuffer() {
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &colorbuffer);


    vertexbuffer_size = 6 * 6 * 7 * 2 + 7 * 6 + 16 * 3;

    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            float px = x * oneSquareX + spaceNorm * oneSquareX - 1.0f;
            float py = y * oneSquareY + spaceNorm * oneSquareY - 1.0f;


            for (int i = 0; i < 6; i++)
            {
                g_vertex_buffer_data[18 * (x + 7 * y) + 3 * i] = px + rect_vertex_buffer[3 * i] * oneSquareX * (1 - spaceNorm * 2);
                g_vertex_buffer_data[18 * (x + 7 * y) + 3 * i + 1] = py + rect_vertex_buffer[3 * i + 1] * oneSquareY * (1 - spaceNorm * 2);
                g_vertex_buffer_data[18 * (x + 7 * y) + 3 * i + 2] = rect_vertex_buffer[3 * i + 2];
                g_color_buffer_data[18 * (x + 7 * y) + 3 * i] = colors[NOTHING];
                g_color_buffer_data[18 * (x + 7 * y) + 3 * i + 1] = colors[NOTHING + 1];
                g_color_buffer_data[18 * (x + 7 * y) + 3 * i + 2] = colors[NOTHING + 2];
            }

            px = x * oneSquareX + spaceInnerPlate * oneSquareX - 1.0f;
            py = y * oneSquareY + spaceInnerPlate * oneSquareY - 1.0f;

            for (int i = 0; i < 6; i++)
            {
                g_vertex_buffer_data[756 + 18 * (x + 7 * y) + 3 * i] = px + coin_vertex_buffer[3 * i] * oneSquareX * (1 - spaceInnerPlate * 2);
                g_vertex_buffer_data[756 + 18 * (x + 7 * y) + 3 * i + 1] = py + coin_vertex_buffer[3 * i + 1] * oneSquareY * (1 - spaceInnerPlate * 2);
                g_vertex_buffer_data[756 + 18 * (x + 7 * y) + 3 * i + 2] = coin_vertex_buffer[3 * i + 2];
                g_color_buffer_data[756 + 18 * (x + 7 * y) + 3 * i] = colors[NOTHING];
                g_color_buffer_data[756 + 18 * (x + 7 * y) + 3 * i + 1] = colors[NOTHING + 1];
                g_color_buffer_data[756 + 18 * (x + 7 * y) + 3 * i + 2] = colors[NOTHING + 2];
            }



        }

        for (int x = 0; x < 7; x++) {

            float px = x * oneSquareX + spaceInnerPlate * oneSquareX - 1.0f;
            float py = 6 * oneSquareY + spaceFirstLine * oneSquareY - 1.0f;

            for (int i = 0; i < 6; i++)
            {
                g_vertex_buffer_data[1512 + 18 * x + 3 * i] = px + coin_vertex_buffer[3 * i] * oneSquareX * (1 - spaceInnerPlate * 2);
                g_vertex_buffer_data[1512 + 18 * x + 3 * i + 1] = py + coin_vertex_buffer[3 * i + 1] * oneSquareY * (1 - spaceInnerPlate * 2);
                g_vertex_buffer_data[1512 + 18 * x + 3 * i + 2] = coin_vertex_buffer[3 * i + 2];
                g_color_buffer_data[1512 + 18 * x + 3 * i] = colors[BACKGROUND];
                g_color_buffer_data[1512 + 18 * x + 3 * i + 1] = colors[BACKGROUND + 1];
                g_color_buffer_data[1512 + 18 * x + 3 * i + 2] = colors[BACKGROUND + 2];
            }
        }

        for (int i = 0; i < 3 * 2; i++) // Color
        {
            g_vertex_buffer_data[1638 + 3 * i] = ((1.6f / 5) * rect_vertex_buffer[3 * i] + (1.6f / 5) * 0) - 0.8f;//-0.8f + (1.6f / 5) * 1 * rect_vertex_buffer[3 * i];
            g_vertex_buffer_data[1638 + 3 * i + 1] = 0.95f - 0.2f * rect_vertex_buffer[3 * i + 1];
            g_vertex_buffer_data[1638 + 3 * i + 2] = rect_vertex_buffer[3 * i + 2];
            g_color_buffer_data[1638 + 3 * i] = colors[BACKGROUND];
            g_color_buffer_data[1638 + 3 * i + 1] = colors[BACKGROUND + 1];
            g_color_buffer_data[1638 + 3 * i + 2] = colors[BACKGROUND + 2];
        }
        for (int i = 0; i < 3 * 2; i++) // W_1
        {
            g_vertex_buffer_data[1656 + 3 * i] = ((1.6f / 5) * W_Char_back[3 * i] + (1.6f / 5) * 1) - 0.8f;;
            g_vertex_buffer_data[1656 + 3 * i + 1] = 0.95f - 0.2f * W_Char_back[3 * i + 1];
            g_vertex_buffer_data[1656 + 3 * i + 2] = W_Char_back[3 * i + 2];
            g_color_buffer_data[1656 + 3 * i] = colors[BACKGROUND];
            g_color_buffer_data[1656 + 3 * i + 1] = colors[BACKGROUND + 1];
            g_color_buffer_data[1656 + 3 * i + 2] = colors[BACKGROUND + 2];
        } 

        for (int i = 0; i < 3 * 2; i++) // W_2
        {
            g_vertex_buffer_data[1674 + 3 * i] = ((1.6f / 5) * W_Char_front[3 * i] + (1.6f / 5) * 1) - 0.8f;
            g_vertex_buffer_data[1674 + 3 * i + 1] = 0.95f - 0.2f * W_Char_front[3 * i + 1];
            g_vertex_buffer_data[1674 + 3 * i + 2] = W_Char_front[3 * i + 2];
            g_color_buffer_data[1674 + 3 * i] = colors[BACKGROUND];
            g_color_buffer_data[1674 + 3 * i + 1] = colors[BACKGROUND + 1];
            g_color_buffer_data[1674 + 3 * i + 2] = colors[BACKGROUND + 2];
        }

        for (int i = 0; i < 3 * 2; i++) // I_1
        {
            g_vertex_buffer_data[1692 + 3 * i] = ((1.6f / 5) * I_Char[3 * i] + (1.6f / 5) * 2) - 0.8f;
            g_vertex_buffer_data[1692 + 3 * i + 1] = 0.95f - 0.2f * I_Char[3 * i + 1];
            g_vertex_buffer_data[1692 + 3 * i + 2] = I_Char[3 * i + 2];
            g_color_buffer_data[1692 + 3 * i] = colors[BACKGROUND];
            g_color_buffer_data[1692 + 3 * i + 1] = colors[BACKGROUND + 1];
            g_color_buffer_data[1692 + 3 * i + 2] = colors[BACKGROUND + 2];
        }

        for (int i = 0; i < 3 * 3; i++) // N_1
        {
            g_vertex_buffer_data[1710 + 3 * i] = ((1.6f / 5) * N_Char[3 * i] + (1.6f / 5) * 3) - 0.8f;
            g_vertex_buffer_data[1710 + 3 * i + 1] = 0.95f - 0.2f * N_Char[3 * i + 1];
            g_vertex_buffer_data[1710 + 3 * i + 2] = N_Char[3 * i + 2];
            g_color_buffer_data[1710 + 3 * i] = colors[BACKGROUND];
            g_color_buffer_data[1710 + 3 * i + 1] = colors[BACKGROUND + 1];
            g_color_buffer_data[1710 + 3 * i + 2] = colors[BACKGROUND + 2];
        }

        for (int i = 0; i < 3; i++) // N_2
        {
            g_vertex_buffer_data[1737 + 3 * i] = ((1.6f / 5) * N_Char_front[3 * i] + (1.6f / 5) * 3) - 0.8f;
            g_vertex_buffer_data[1737 + 3 * i + 1] = 0.95f - 0.2f * N_Char_front[3 * i + 1];
            g_vertex_buffer_data[1737 + 3 * i + 2] = N_Char_front[3 * i + 2];
            g_color_buffer_data[1737 + 3 * i] = colors[BACKGROUND];
            g_color_buffer_data[1737 + 3 * i + 1] = colors[BACKGROUND + 1];
            g_color_buffer_data[1737 + 3 * i + 2] = colors[BACKGROUND + 2];
        }

        for (int i = 0; i < 3 * 4; i++) // S_1
        {
            g_vertex_buffer_data[1746 + 3 * i] = ((1.6f / 5) * S_Char[3 * i] + (1.6f / 5) * 4) - 0.8f;
            g_vertex_buffer_data[1746 + 3 * i + 1] = 0.95f - 0.2f * S_Char[3 * i + 1];
            g_vertex_buffer_data[1746 + 3 * i + 2] = S_Char[3 * i + 2];
            g_color_buffer_data[1746 + 3 * i] = colors[BACKGROUND];
            g_color_buffer_data[1746 + 3 * i + 1] = colors[BACKGROUND + 1];
            g_color_buffer_data[1746 + 3 * i + 2] = colors[BACKGROUND + 2];
        }


    }
}

void drawWin(int color) {
    for (int i = 0; i < 3 * 2; i++) // Color
    {
        g_color_buffer_data[1638 + 3 * i] = colors[color];
        g_color_buffer_data[1638 + 3 * i + 1] = colors[color + 1];
        g_color_buffer_data[1638 + 3 * i + 2] = colors[color + 2];
    }
    for (int i = 0; i < 3 * 2; i++) // W_1
    {
        g_color_buffer_data[1656 + 3 * i] = colors[GREY];
        g_color_buffer_data[1656 + 3 * i + 1] = colors[GREY + 1];
        g_color_buffer_data[1656 + 3 * i + 2] = colors[GREY + 2];
    }

    for (int i = 0; i < 3 * 2; i++) // W_2
    {
        g_color_buffer_data[1674 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1674 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1674 + 3 * i + 2] = colors[BACKGROUND + 2];
    }

    for (int i = 0; i < 3 * 2; i++) // I_1
    {;
        g_color_buffer_data[1692 + 3 * i] = colors[GREY];
        g_color_buffer_data[1692 + 3 * i + 1] = colors[GREY + 1];
        g_color_buffer_data[1692 + 3 * i + 2] = colors[GREY + 2];
    }

    for (int i = 0; i < 3 * 3; i++) // N_1
    {
        g_color_buffer_data[1710 + 3 * i] = colors[GREY];
        g_color_buffer_data[1710 + 3 * i + 1] = colors[GREY + 1];
        g_color_buffer_data[1710 + 3 * i + 2] = colors[GREY + 2];
    }

    for (int i = 0; i < 3; i++) // N_2
    {
        g_color_buffer_data[1737 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1737 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1737 + 3 * i + 2] = colors[BACKGROUND + 2];
    }

    for (int i = 0; i < 3 * 4; i++) // S_1
    {
        g_color_buffer_data[1746 + 3 * i] = colors[GREY];
        g_color_buffer_data[1746 + 3 * i + 1] = colors[GREY + 1];
        g_color_buffer_data[1746 + 3 * i + 2] = colors[GREY + 2];
    }
}

void clearWin() {
    for (int i = 0; i < 3 * 2; i++) // Color
    {
        g_color_buffer_data[1638 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1638 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1638 + 3 * i + 2] = colors[BACKGROUND + 2];
    }
    for (int i = 0; i < 3 * 2; i++) // W_1
    {
        g_color_buffer_data[1656 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1656 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1656 + 3 * i + 2] = colors[BACKGROUND + 2];
    }

    for (int i = 0; i < 3 * 2; i++) // W_2
    {
        g_color_buffer_data[1674 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1674 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1674 + 3 * i + 2] = colors[BACKGROUND + 2];
    }

    for (int i = 0; i < 3 * 2; i++) // I_1
    {
        ;
        g_color_buffer_data[1692 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1692 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1692 + 3 * i + 2] = colors[BACKGROUND + 2];
    }

    for (int i = 0; i < 3 * 3; i++) // N_1
    {
        g_color_buffer_data[1710 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1710 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1710 + 3 * i + 2] = colors[BACKGROUND + 2];
    }

    for (int i = 0; i < 3; i++) // N_2
    {
        g_color_buffer_data[1737 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1737 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1737 + 3 * i + 2] = colors[BACKGROUND + 2];
    }

    for (int i = 0; i < 3 * 4; i++) // S_1
    {
        g_color_buffer_data[1746 + 3 * i] = colors[BACKGROUND];
        g_color_buffer_data[1746 + 3 * i + 1] = colors[BACKGROUND + 1];
        g_color_buffer_data[1746 + 3 * i + 2] = colors[BACKGROUND + 2];
    }
}

bool initializeVertexbuffer()
{
    
    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            for (int i = 0; i < 6; i++)
            {
                g_color_buffer_data[756 + 18 * (x + 7 * y) + 3 * i] = colors[gameBoard->getField(x, y)];
                g_color_buffer_data[756 + 18 * (x + 7 * y) + 3 * i + 1] = colors[gameBoard->getField(x, y) + 1];
                g_color_buffer_data[756 + 18 * (x + 7 * y) + 3 * i + 2] = colors[gameBoard->getField(x, y) + 2];
            }
        }
    }

    //glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

