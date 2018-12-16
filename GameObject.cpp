#include "GameObject.h"
#include "Component.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentLight.h"
#include "ModuleInput.h"
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
	this->id = App->scene->GOcounter;
	this->name = name;
	this->active = active;
	this->parent = App->scene->Root;
}


GameObject::GameObject(char* name,bool active, GameObject* parent)
{
	this->id = App->scene->GOcounter;
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

	ImGuiTreeNodeFlags node_flags = (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected);

	if (ImGui::TreeNodeEx((void*)0,node_flags, "%s %i", this->name, this->id))
	{
		if (ImGui::IsItemHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->scene->SelectedGO = this;
		}
		//Draw your children
		for (std::list<GameObject*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
		{
			(*it)->DrawHierarchy();
		}

		ImGui::TreePop();
	}
	
}

void GameObject::DrawComponents(int type)
{
	switch (type)
	{
	case MESH:
		this->DrawMeshes();
		break;
	case MATERIAL:
		this->DrawMaterials();
		break;
	case TRANSFORM:
		this->DrawTransforms();
		break;
	case LIGHT:
		this->DrawLights();
		break;
	}
}

void GameObject::DrawMeshes()
{
	int i = 0;
	for (std::vector<Component*>::iterator it = this->MeshComponents.begin(); it != this->MeshComponents.end(); ++it, ++i)
	{
		ImGui::Text("Component Mesh %i", i);
	}
}

void GameObject::DrawMaterials()
{
	int i = 0;
	for (std::vector<Component*>::iterator it = this->MaterialComponents.begin(); it != this->MaterialComponents.end(); ++it, ++i)
	{
		ImGui::Text("Component Material %i", i);
	}
}

void GameObject::DrawTransforms()
{
	int i = 0;
	for (std::vector<Component*>::iterator it = this->TransformComponents.begin(); it != this->TransformComponents.end(); ++it, ++i)
	{
		ImGui::Text("Component Transform %i", i);
	}
}

void GameObject::DrawLights()
{
	int i = 0;
	for (std::vector<Component*>::iterator it = this->LightComponents.begin(); it != this->LightComponents.end(); ++it, ++i)
	{
		ImGui::Text("Component Light %i", i);
	}
}

Component* GameObject::CreateComponent(int type)
{
	Component* comp = nullptr;

	switch (type) 
	{
	case MESH:
		comp = new(ComponentMesh);
		this->MeshComponents.push_back(comp);
		break;
	case MATERIAL:
		comp = new(ComponentMaterial);
		this->MaterialComponents.push_back(comp);
		break;
	case LIGHT:
		comp = new(ComponentLight);
		this->LightComponents.push_back(comp);
		break;
	case TRANSFORM: 	
		this->TransformComponents.push_back(comp);
		comp = new(ComponentTransform);
		break;
	}
	this->components.push_back(comp);
	return comp;
}


void GameObject::Update()
{

}
