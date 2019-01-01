#ifndef __MODULESCENE_H_
#define __MODULESCENE_H_

#include <list>
#include "Module.h"
#include "ModuleModelLoader.h"
#include "ModuleEditor.h"

class GameObject;

class ModuleScene :	public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Draw();

	GameObject* CreateGameObject(const char* name, bool active, GameObject* parent, const char* FileLocation);
	GameObject* CreateGameObject(const char* name, bool active, const char* FileLocation);
	void DuplicateGameObject(GameObject* GO);

	GameObject* Root = nullptr;
	GameObject* SelectedGO = nullptr;

	int GOcounter = 1;

	std::list<GameObject*> GOs; //map or list or vector??

	ModuleScene();
	~ModuleScene();



private:




};

#endif // __MODULESCENE_H_