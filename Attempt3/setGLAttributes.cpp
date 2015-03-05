#include "variables.h"



SetAttributes::SetAttributes(int major, int minor)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	std::cout << "Set OpenGL and context to version " << major << "." << minor << " OK!\n";
}


SetAttributes::~SetAttributes()
{
}
