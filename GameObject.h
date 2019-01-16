#ifndef __GAMEOBJECT_H_
#define __GAMEOBJECT_H_

#include "Globals.h"
#include "Component.h"
#include "ModuleProgram.h"
#include "assimp/material.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Math/MathFunc.h"
#include "Geometry/AABB.h"
#include "Config.h"
#include "debugdraw.h"
#include <vector>
#include<list>

class Component;
class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;
class ComponentLight;

class GameObject
{
public:


	GameObject();
	GameObject(const char* name, bool active, const char* FileLocation);
	GameObject(const char* name, bool active, GameObject* parent, const char* FileLocation);

	~GameObject();

	void Update();
	Component* CreateComponent(int type);
	Component* DuplicateComponent(int type, GameObject* GO);

	void Save(Config* config);


	AABB LoadBB();


	bool active = true;
	const char* name = nullptr;
	const char* UUID = nullptr;
	int id =0;
	GameObject* parent = nullptr;
	const char*	filePath = nullptr;
	char newName[20];
	AABB BB = AABB();
	bool Static = true;
	bool rendered = true;

	bool showPopup = false;

	std::string getFileFolder();
	void ModelTransform(unsigned shader) const;
	float4x4 GetGlobalTransform() const;
	float4x4 GetLocalTransform() const;


	void BeginPopup(GameObject* GO);
	void Draw();
	void DrawBB();
	void DrawComponents(int type);
	void DrawHierarchy();
	void DrawMeshes();
	void DrawMaterials();
	void DrawTransforms();
	void DrawLights();


	std::list<GameObject*> children;
	std::vector<Component*> components;

	std::vector<Component*> MeshComponents;
	std::vector<Component*> MaterialComponents;
	std::vector<Component*> TransformComponents;
	std::vector<Component*> LightComponents;

	ComponentMesh* mesh = nullptr;
	ComponentMaterial* material = nullptr;
	ComponentTransform* transform = nullptr;
	ComponentLight* light = nullptr;

};

#endif // __GAMEOBJECT_H_