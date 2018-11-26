#pragma once
#include<list>
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	update_status Update();
	bool CleanUp();

	GLuint loadImg(const char* path);

private:
	unsigned texture0 = 0;
};