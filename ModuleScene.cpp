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

void ModuleScene::Draw()
{
	Root->Draw();
}

void ModuleScene::DuplicateGameObject(GameObject* GO)
{
	
	App->modelLoader->LoadFBX(GO->mesh->path);

	//Copy its children
	
	for (std::list<GameObject*>::iterator it = GO->children.begin(); it != GO->children.end(); ++it)
	{
		DuplicateGameObject(*it);
	}

	//return my_go;
}

GameObject* ModuleScene::CreateGameObject(const char* name, bool active, GameObject* parent, const char* FileLocation)
{
	GameObject* my_go = new GameObject(name, active, parent,FileLocation);
	parent->children.push_back(my_go);
	GOs.push_back(my_go);

	return my_go;
}

GameObject* ModuleScene::CreateGameObject(const char* name, bool active, const char* FileLocation)
{
	GameObject* my_go = new GameObject(name, active, FileLocation);
	Root->children.push_back(my_go);
	GOs.push_back(my_go);
	return my_go;
}
