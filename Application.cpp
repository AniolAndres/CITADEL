#include "Application.h"
#include "Brofiler.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleDebugDraw.h"
#include "ModuleTextures.h"
#include "Crossguid/crossguid/guid.hpp"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleFileSystem.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "string.h"
#include "ModuleModelLoader.h"


using namespace std;

Application::Application()
{
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(modelLoader = new ModuleModelLoader());
	modules.push_back(debugDraw = new ModuleDebugDraw());
	modules.push_back(fileSystem = new ModuleFileSystem());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PreUpdate();
	BROFILER_CATEGORY("PreUpdate", Profiler::Color::Orchid)
	}

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->Update();
	BROFILER_CATEGORY("Update", Profiler::Color::OrangeRed)
	}
	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PostUpdate();
	BROFILER_CATEGORY("PostUpdate", Profiler::Color::Peru)
	}
		

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

const char* Application::GenerateUUID()
{
	string uuid = xg::newGuid().str();
	char* newUUID = new char[strlen(uuid.c_str())];
	strcpy(newUUID, uuid.c_str());
	return newUUID;
}