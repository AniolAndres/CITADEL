#include "GameObject.h"
#include "Component.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Globals.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl.h"
#include "IMGUI/imgui_impl_opengl2.h"
#include<vector>
#include<list>

GameObject::GameObject()
{

}

GameObject::GameObject(char* name, bool active)
{
	//this->components.reserve(5);
	//this->components.push_back(mesh = new ComponentMesh());
	//this->components.push_back(material = new ComponentMaterial());
	//this->components.push_back(transform = new ComponentTransform());
	//this->components.push_back(light = new ComponentLight());

	this->name = name;
	this->active = active;
	this->parent = App->scene->Root;
}


GameObject::GameObject(char* name,bool active, GameObject* parent)
{
	//this->components.reserve(5);
	//this->components.push_back(mesh = new ComponentMesh());
	//this->components.push_back(material = new ComponentMaterial());
	//this->components.push_back(transform = new ComponentTransform());
	//this->components.push_back(light = new ComponentLight());

	this->name = name;
	this->active = active;
	this->parent = parent;
}


GameObject::~GameObject()
{
}


void GameObject::Draw()
{
	//Draw yourself

	/*this->Draw();*/

	//Draw your children

	for (std::list<GameObject*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
	{
		/*(*it)->Draw();*/
	}
}

void GameObject::DrawHierarchy()
{
	//Draw yourself
	/*ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;*/


	//-----------------------

	if (ImGui::TreeNode(this->name))
	{
		App->scene->SelectedGO = this;

		//Draw your children
		for (std::list<GameObject*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
		{
			(*it)->DrawHierarchy();
		}
		ImGui::TreePop();
	}
	
}

Component* GameObject::CreateComponent(int type)
{
	Component* comp = nullptr;

	switch (type) 
	{
	case MESH:
		comp = new(ComponentMesh);
		break;
	case MATERIAL:
		comp = new(ComponentMaterial);
		break;
	case LIGHT:
		comp = new(ComponentLight);
		break;
	case TRANSFORM: 
		comp = new(ComponentTransform);
		break;
	}
	return comp;
}


void GameObject::Update()
{

}
