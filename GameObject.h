#ifndef __GAMEOBJECT_H_
#define __GAMEOBJECT_H_

#include "Globals.h"
#include "Component.h"
#include "ModuleProgram.h"
#include "assimp/material.h"
#include "MathGeoLib/include/Math/float4x4.h"
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
	GameObject(GameObject* GO);

	~GameObject();

	void Update();
	Component* CreateComponent(int type);
	GameObject* parent = nullptr;

	bool active;
	const char* name;
	int id;
	const char*	filePath = nullptr;
	char newName[20];

	bool showPopup = false;

	std::string getFileFolder();
	void ModelTransform(unsigned shader) const;
	float4x4 GetGlobalTransform() const;
	float4x4 GetLocalTransform() const;

	void BeginPopup(GameObject* GO);
	void Draw();
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