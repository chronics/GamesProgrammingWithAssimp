#include "variables.h"


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

	
	
}




createWindow::~createWindow()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}
