#include <iostream>
#include "setGLAttributes.h"

using namespace std;

setGLAttributes::setGLAttributes(int major, int minor)
{
	
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
		cout << "Set OpenGL context to version " << major << "." << minor << " OK!\n";
}



setGLAttributes::~setGLAttributes()
{
}
