#include "GameObject.h"
#include "Component.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Globals.h"
#include<vector>

GameObject::GameObject()
{

}

GameObject::GameObject(char* name, bool active)
{
	this->components.reserve(5);
	this->components.push_back(mesh = new ComponentMesh());
	this->components.push_back(material = new ComponentMaterial());
	this->components.push_back(transform = new ComponentTransform());
	this->components.push_back(light = new ComponentLight());

	this->name = name;
	this->active = active;
	this->hasParent = false;
}


GameObject::GameObject(char* name,bool active, GameObject* parent)
{
	this->components.reserve(5);
	this->components.push_back(mesh = new ComponentMesh());
	this->components.push_back(material = new ComponentMaterial());
	this->components.push_back(transform = new ComponentTransform());
	this->components.push_back(light = new ComponentLight());

	this->name = name;
	this->active = active;
	this->parent = parent;
	this->hasParent = true;

}


GameObject::~GameObject()
{
}


//Component* GameObject::CreateComponent(int type)
//{
//	Component* comp = nullptr;
//
//	switch (type) 
//	{
//	case MESH:
//		comp = new(ComponentMesh);
//		break;
//	case MATERIAL:
//		comp = new(ComponentMaterial);
//		break;
//	case LIGHT:
//		comp = new(ComponentLight);
//		break;
//	case TRANSFORM: 
//		comp = new(ComponentTransform);
//		break;
//	}
//	return comp;
//}


void GameObject::Update()
{

}
