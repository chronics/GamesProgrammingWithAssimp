#include <iostream>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <fstream>
#include <string>
//include header files
#include "createWindow.h"
#include "setGLAttributes.h"
#include "loadAssets.h"


using namespace std;

//Variables

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
	
	createWindow createWindow("GamesEngineV2", 1366, 768);
	setGLAttributes setGLAttributes(3, 3);

	loadAssets loadAssets();

	SDL_Delay(10000);

	SDL_Quit();

	return 0;
}