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


class createWindow
{
public:
	createWindow(const std::string& title, int width, int height);
	~createWindow();

	void createContext();
	void initGlew();

private:
	SDL_Window* window;
	SDL_GLContext context;
};

