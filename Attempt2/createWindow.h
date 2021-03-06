#ifndef _CREATEWINDOW_H
#define _CREATEWINDOW_H

#include <string>

#include <SDL2/SDL.h>

class createWindow
{
public:
	createWindow(const std::string& title, int width, int height);
	~createWindow();

private:
	SDL_Window* window;
	SDL_GLContext context;
};

#endif
