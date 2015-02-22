#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <string>
#include <SDL2/SDL.h>

class Display
{
public:
	Display(const std::string& title, int width, int height);
	~Display();

	void Resize(int width, int height);
	void Fullscreen(bool flag);

private:
	SDL_Window* window;
	SDL_GLContext context;
};

#endif