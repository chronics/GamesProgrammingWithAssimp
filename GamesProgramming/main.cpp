#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
//include headers
#include "Display.h"
#include "setGLAttributes.h"
#include "vertexData.h"

using namespace std;

GLuint vertexBufferObject;
GLuint vertexBufferObject2D;
GLuint vao;

void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}

void initializeVertexBuffer()
{

	/*
	glGenBuffers(1, &vertexBufferObject2D);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject2D);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData2D), vertexData2D, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "positionBufferObject2D created OK! GLUint is: " << vertexBufferObject << std::endl;
	*/

	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "positionBufferObject created OK! GLUint is: " << vertexBufferObject << std::endl;
}

int main(int argc, char *argv[])
{
	initialise();

	/*
	createWindow();
	setGLAttributes();
	createContext();
	initGlew();
	*/

	setGLAttributes setGLAttributes(3,3); // this will set the OpenGL version to 3.3

	Display display("Games Programming", 1080, 768);

	SDL_Delay(10000);

	SDL_Quit();

	return 0;
}