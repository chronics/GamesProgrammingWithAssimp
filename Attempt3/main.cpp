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

#include "variables.h"

using namespace std;

bool done = false;

void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}

int /*mainClass::*/main( int argc, char* args[] )
{
	//do once
	initialise();
	createWindow::createWindow("gamesProgramming", 600, 600); // set name, hight and width
	SetAttributes::SetAttributes(3, 3);
	CreateContext::CreateContext();

	loadAssets::loadAssets();

	while (!done && (SDL_GetTicks() < 500000)) 
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

		//render::render(); //RENDER HERE - PLACEHOLDER

		SDL_GL_SwapWindow(window);; //present the frame buffer to the display (swapBuffers)

	} 

	return 0;
}