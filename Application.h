#ifndef __Application_H_
#define __Application_H_

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleCamera;
class ModuleEditor;
class ModuleModelLoader;
class ModuleScene;
class ModuleDebugDraw;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleModelLoader* modelLoader = nullptr;
	ModuleScene* scene = nullptr;
	ModuleDebugDraw* debugDraw = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;

#endif // __Application_H_