#ifndef __MODULESCENE_H_
#define __MODULESCENE_H_

#include<list>
#include "Module.h"

class GameObject;

class ModuleScene :	public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	GameObject* CreateGameObject(char* name, bool active, GameObject* parent);

	ModuleScene();
	~ModuleScene();

private:

	std::list<GameObject*> GOs;


};

#endif // __MODULESCENE_H_