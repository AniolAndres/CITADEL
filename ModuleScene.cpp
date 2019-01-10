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

void ModuleScene::LoadGeometry(GameObject* container, GeometryType Gtype)
{
	par_shapes_mesh_s* pmesh = nullptr;

	switch (Gtype)
	{
	case SPHERE:
		pmesh = par_shapes_create_parametric_sphere(30, 30);
		break;
	case CUBE:
		pmesh = par_shapes_create_cube();
		break;
	case TORUS:
		pmesh = par_shapes_create_torus(30, 30, 1.f);
		break;
	}

	if (pmesh != nullptr)
	{
		par_shapes_scale(pmesh, 5.0f, 5.0f, 5.0f);

		ComponentMesh* mesh = (ComponentMesh*)container->CreateComponent(MESH);
		mesh->LoadMesh(pmesh);
		par_shapes_free_mesh(pmesh);
		
		ComponentMaterial* mat = (ComponentMaterial*)container->CreateComponent(MATERIAL);
		mat->LoadMaterial(nullptr);
		container->LoadBB();
	}

}

void ModuleScene::DuplicateGameObject(GameObject* GO)
{
	
	App->modelLoader->LoadFBX(GO->mesh->path);

	//Copy its children
	
	for (std::list<GameObject*>::iterator it = GO->children.begin(); it != GO->children.end(); ++it)
	{
		DuplicateGameObject(*it);
	}
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
