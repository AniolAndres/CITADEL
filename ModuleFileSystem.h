#ifndef __MODULEFILESYSTEM_H_
#define __MODULEFILESYSTEM_H_

#include "Module.h"

class ModuleFileSystem : public Module
{
public:

	bool Init();

	update_status PreUpdate();

	update_status Update();

	update_status PostUpdate();

	bool CleanUp();

	ModuleFileSystem();
	~ModuleFileSystem();
};

#endif //__MODULEFILESYSTEM_H_