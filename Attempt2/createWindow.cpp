#include "createWindow.h"

#include <iostream>

#include <GL\glew.h>
#include <SDL2\SDL.h>


createWindow::createWindow(const std::string& title, int width, int height)
{

	std::cout << "Fullscreen will be set to 768p\nFullscreen? (y/n): ";

	char inputc = 'y';
	std::cin >> inputc;
	std::cout << "Loading... (this may take a while!) ...\n";

	if (inputc == 'y' || inputc == 'Y'){
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);
	}
	else
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

	if (window == nullptr)
		std::cout << "Failed to create window!" << std::endl;


	//CreateContext
	context = SDL_GL_CreateContext(window);
	if (context == nullptr){
		SDL_DestroyWindow(window);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}


	//initGlew
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev){
		std::cout << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}


createWindow::~createWindow()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}
