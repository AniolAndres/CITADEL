#ifndef __GAMEOBJECT_H_
#define __GAMEOBJECT_H_

#include "Globals.h"
#include "Component.h"
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
	GameObject(char* name, bool active);
	GameObject(char* name, bool active, GameObject* parent);
	~GameObject();

	void Update();
	/*Component* CreateComponent(int type);*/
	GameObject* parent;

	bool hasParent;
	bool active;
	char* name;

	std::list<GameObject*> children;
	std::vector<Component*> components;

	ComponentMesh* mesh = nullptr;
	ComponentMaterial* material = nullptr;
	ComponentTransform* transform = nullptr;
	ComponentLight* light = nullptr;

};

#endif // __GAMEOBJECT_H_