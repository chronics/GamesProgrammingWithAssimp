#ifndef _RENDER_H
#define _RENDER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <GL\glew.h>

#define GLM_FORCE_RADIANS

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>



class render
{
public:
	render();
	~render();

	GLint positionLocation; //GLint that we'll fill in with the location of the `position` attribute in the GLSL
	GLint colorLocation; //GLint that we'll fill in with the location of the `color` attribute in the GLSL
	GLint modelMatrixLocation; //GLint that we'll fill in with the location of the `modelMatrix` uniform in the GLSL
	GLint viewMatrixLocation;

};

#endif