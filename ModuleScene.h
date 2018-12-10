#ifndef __MODULESCENE_H_
#define __MODULESCENE_H_

#include<list>
#include "Module.h"
#include "GameObject.h"

class ModuleScene :	public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	GameObject* CreateGameObject();

private:

	std::list<GameObject*> GOs;

	ModuleScene();
	~ModuleScene();
};

#endif // __MODULESCENE_H_