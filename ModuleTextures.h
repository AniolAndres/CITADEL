#ifndef __ModuleTextures_h__
#define __ModuleTextures_h__

#include<list>
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

struct Texture
{
	int id = 0;
	int width = 0;
	int height = 0;
	Texture(int id, int width, int height) : id(id), width(width), height(height) { }
};

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	update_status Update();
	bool CleanUp();

	Texture* loadImg(const char* path);

	bool mipmaping = false;
	int filterType = GL_LINEAR;
	int mipMapMode = GL_NEAREST_MIPMAP_NEAREST;
	int wrapMode = GL_CLAMP;
	Texture* defaultTexture = nullptr;

private:
	unsigned texture0 = 0;
};

#endif // __ModuleTextures_h__