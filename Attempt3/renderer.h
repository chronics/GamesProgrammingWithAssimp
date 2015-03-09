#pragma once
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

class renderer
{
public:
	SDL_Window* window;
	SDL_GLContext context;

	renderer(const std::string& title, int width, int height, int major, int minor);
	~renderer();


private:

	void createWindow(const std::string& title, int width, int height);
	void createContext();
	void setAttributes(int major, int minor);
};
