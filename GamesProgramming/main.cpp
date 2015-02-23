#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <fstream>

//include headers
#include "createWindow.h"
#include "vertexData.h"
#include "setGLAttributes.h"
#include "LoadAssets.h"

using namespace std;

//ifstream vertexShader("VertexShader.txt");
//ifstream fragmentShader("FragmentShader.txt");

void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}

int main(int argc, char *argv[])
{
	initialise();
	createWindow createWindow("Games Programming", 1280, 720); // this will set the program info (name, x, y)
	setGLAttributes setGLAttributes(3, 3);
	
	LoadAssets LoadAssets(ifstream vertexShader, ifstream fragmentShader);
	
	glClearColor(1.0f,0.0f,0.0f,1.0f);

	SDL_Delay(10000);

	SDL_Quit();

	return 0;
}

