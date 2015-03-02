#ifndef _LOADASSEST_H
#define _LOADASSEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include <SDL2\SDL.h>
#include <GL\glew.h>

#define GLM_FORCE_RADIANS

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

class loadAssets
{
public:
	loadAssets();
	~loadAssets();
	GLuint vao, vertexBufferObject, theProgram, vertex, normal, uv;
	GLint positionLocation, colorLocation, modelMatrixLocation, viewMatrixLocation;

	glm::mat4 modelMatrix; // the modelMatrix for our object - which is the identity matrix by default
	glm::mat4 viewMatrix; // the viewMatrix for our "camera" - which is the identity matrix by default

	
};

#endif