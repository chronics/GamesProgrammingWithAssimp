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

class windowVars
{
public:
	windowVars();
	~windowVars();

	SDL_Window* window;
	SDL_GLContext context;

private:

};

/*
class mainClass : public windowVars
{
public:
	mainClass();
	~mainClass();

	int main(int argc, char* args[]);

private:

};
*/
class loadAssets
{
public:
	
	GLuint theProgram, vertexBufferObject, vertexBufferObject2D, vao;
	GLint positionLocation, colorLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;

	glm::mat4 modelMatrix, viewMatrix, projectionMatrix, rotationMatrix, translationMatrix;

	loadAssets();
	~loadAssets();

	void initializeProgram();
	void initializeVertexBuffer();
	
private:

};

class createWindow : public windowVars
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

class CreateContext : public windowVars
{
public:
	CreateContext();
	~CreateContext();

	void initGlew();
private:

	
};


class render : public loadAssets
{
public:
	render();
	~render();

private:

};


#endif