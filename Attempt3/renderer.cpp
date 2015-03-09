#include "renderer.h"


renderer::renderer(const std::string& title, int width, int height, int major, int minor)
{
	//error check
	//what if fails
	//what if SDL is already initialised...
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	std::cout << "SDL initialised OK!\n";

	createWindow(title, width, height);
	setAttributes(major, minor);
	createContext();
}


renderer::~renderer()
{
}

void renderer::setAttributes(int major, int minor)
{
	//error checking

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	std::cout << "Set OpenGL and context to version " << major << "." << minor << " OK!\n";
}

void renderer::createWindow(const std::string& title, int width, int height)
{

	std::cout << "Fullscreen will be set to 768p\nFullscreen? (y/n): ";

	char inputc = 'y';
	std::cin >> inputc;
	std::cout << "Loading... (this may take a while!) ...\n";
	std::cout << "You want your freedom?\nTake it\nThat's what I'm counting on\nI used to want you dead\nbut\nNow I only want you gone";

	if (inputc == 'y' || inputc == 'Y'){
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);
	}
	else
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

	if (window == nullptr)
		std::cout << "Failed to create window!" << std::endl;

}

void renderer::createContext()
{
	context = SDL_GL_CreateContext(window);
	if (context == nullptr){
		SDL_DestroyWindow(window);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	initGlew();
}


void initGlew()
{
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