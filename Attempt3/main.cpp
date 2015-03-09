#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#define GLM_FORCE_RADIANS 

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

//#include "variables.h"
#include "renderer.h"

using namespace std;

bool done = false;
renderer *myRenderer;



GLuint theProgram, vertexBufferObject, vertexBufferObject2D, vao;
GLint positionLocation, colorLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;

glm::mat4 modelMatrix, viewMatrix, projectionMatrix, rotationMatrix, translationMatrix;

void initializeProgram();
void initializeVertexBuffer();


int /*mainClass::*/main( int argc, char* args[] )
{
	//do once
	myRenderer = new renderer("gamesProgramming", 600, 600, 3, 3);

	//TODO loadAssets::loadAssets();

	while (!done && (SDL_GetTicks() < 500000)) 
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

		renderer::render(); //RENDER HERE - PLACEHOLDER

		renderer.swapWindow; //present the frame buffer to the display (swapBuffers)

	} 

	delete myRenderer;
	return 0;
}