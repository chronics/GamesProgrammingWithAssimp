#ifndef _VARIABLES_H
#define _VARIABLES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#define GLM_FORCE_RADIANS 

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

GLuint theProgram, vertexBufferObject, vertexBufferObject2D, vao;
GLint positionLocation, colorLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;

class GLSLGLVariables
{
public:
	GLSLGLVariables();
	~GLSLGLVariables();

	

private:

};

class createWindow
{
public:
	createWindow(const std::string& title, int width, int height);
	~createWindow();

private:
	SDL_Window* window;
	SDL_GLContext context;
};

class SetAttributes
{
public:
	SetAttributes(int major, int minor);
	~SetAttributes();

private:
};


#endif