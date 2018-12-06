#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	////Viewport we'll be rendering to
	//SDL_Window* viewportWindow = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	SDL_version version;

	float windowHeight, windowWidth;
};

#endif // __ModuleWindow_H__