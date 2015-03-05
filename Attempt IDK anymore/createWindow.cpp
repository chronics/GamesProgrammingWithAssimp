#include "createWindow.h"


createWindow::createWindow(const std::string& title, int width, int height)
{
	std::cout << "Fullscreen? (y/n): ";

	char inputc = 'y';
	std::cin >> inputc;
	std::cout << "Loading-- (this may take a while!) ...\n\n";

	if (inputc == 'y' || inputc == 'Y'){
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);
		std::cout << "fullscreen working\n";
	}
	else
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

	if (window == nullptr)
		std::cout << "Failed to create window!" << std::endl;
}

createWindow::createContext()
{
	context = SDL_GL_CreateContext(window);
	if (context == nullptr){
		SDL_DestroyWindow(window);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	std::cout << "Created OpenGL context OK!\n";
}

createWindow::initGlew()
{
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev){
		std::cout << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		std::cout << "GLEW Init OK!\n";
	}
}


createWindow::~createWindow()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}
