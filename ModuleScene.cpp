#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"


ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	bool ret = true;
	Root = new GameObject();
	Root->id = NULL;
	Root->name = "Root";
	Root->active = true;
	return ret;
}

update_status ModuleScene::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	bool ret = true;

	return ret;
}

GameObject* ModuleScene::CreateGameObject(char* name, bool active, GameObject* parent)
{
	GameObject* my_go = new GameObject(name,active,parent);
	parent->children.push_back(my_go);
	GOs.push_back(my_go);

	return my_go;
}

GameObject* ModuleScene::CreateGameObject(char* name, bool active)
{
	GameObject* my_go = new GameObject(name, active);
	Root->children.push_back(my_go);
	GOs.push_back(my_go);
	return my_go;
}
