#include "setGLAttributes.h"

#include <iostream>
#include <SDL2\SDL.h>


setGLAttributes::setGLAttributes(int major, int minor)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	std::cout << "Set OpenGL and context to version " << major << "." << minor << " OK!\n";
}


setGLAttributes::~setGLAttributes()
{
}
