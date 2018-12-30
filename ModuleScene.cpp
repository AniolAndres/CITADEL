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

GameObject* ModuleScene::DuplicateGameObject(GameObject* GO)
{
	GameObject* my_go = new GameObject();

	//Copy the gameobject itself along with every component the original has.
	my_go->name = GO->name;
	GO->parent->children.push_back(my_go);

	if (GO->Static == false)
	{
		//Components
		my_go->mesh = (ComponentMesh*)GO->DuplicateComponent(MESH, GO);
		my_go->material = (ComponentMaterial*)GO->DuplicateComponent(MATERIAL, GO);
		my_go->transform = (ComponentTransform*)GO->DuplicateComponent(TRANSFORM, GO);
	}
	//Copy its children
	
	for (std::list<GameObject*>::iterator it = GO->children.begin(); it != GO->children.end(); ++it)
	{
		GO->children.push_back(DuplicateGameObject((*it)));
	}

	return my_go;
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
