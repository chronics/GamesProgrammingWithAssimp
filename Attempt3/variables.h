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

SDL_Window* window;
SDL_GLContext context;

glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 rotationMatrix; 
glm::mat4 translationMatrix;

class loadAssets
{
public:
	loadAssets();
	~loadAssets();

	void initializeProgram();
	void initializeVertexBuffer();
	
private:

};

class createWindow
{
public:
	createWindow(const std::string& title, int width, int height);
	~createWindow();

	void CreateContext();
	
private:
};

class SetAttributes
{
public:
	SetAttributes(int major, int minor);
	~SetAttributes();

private:
};

class CreateContext
{
public:
	CreateContext();
	~CreateContext();

	void initGlew();
private:
	;
};

class render
{
public:
	render();
	~render();

private:

};


#endif