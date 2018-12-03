#include "ModuleFileSystem.h"
#include "Application.h"
#include "Globals.h"

bool ModuleFileSystem::Init()
{
	bool ret = true;

	return ret;
}

update_status ModuleFileSystem::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleFileSystem::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleFileSystem::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = true;
	return ret;
}

ModuleFileSystem::ModuleFileSystem()
{
}


ModuleFileSystem::~ModuleFileSystem()
{
}
