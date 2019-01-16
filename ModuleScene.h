#ifndef __MODULESCENE_H_
#define __MODULESCENE_H_

#include <list>

#include "Module.h"

#include "ModuleModelLoader.h"
#include "ModuleEditor.h"
#include "Geometry/Frustum.h"
#include "Globals.h"
#include "Config.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"

enum GeometryType
{
	SPHERE=1,
	TORUS,
	CUBE
};

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
	void LoadGeometry(GameObject* parent, GeometryType Gtype);

	void SaveScene();
	void LoadScene();

	void SaveGameObject(Config* config, GameObject* GO);

	GameObject* CreateGameObject(const char* name, bool active, GameObject* parent, const char* FileLocation);
	GameObject* CreateGameObject(const char* name, bool active, const char* FileLocation);
	void CreateGameObject(Config* config, Value &value);

	GameObject* GetGameObjectByUUID(GameObject* GO, const char* name);

	void DuplicateGameObject(GameObject* GO);

	GameObject* Root = nullptr;
	GameObject* SelectedGO = nullptr;
	GameObject* CopiedGO = nullptr;

	int GOcounter = 1;

	float3 lightPosition = { 1000.f, 1000.f, 1000.f };
	float ambientLight = 0.4f;

	std::list<GameObject*> GOs; //map or list or vector??

	ModuleScene();
	~ModuleScene();



private:




};

#endif // __MODULESCENE_H_