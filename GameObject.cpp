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
#include "Assimp/material.h"
#include "Globals.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include<vector>
#include<list>

GameObject::GameObject()
{

}

GameObject::GameObject(const char* name, bool active, const char* FileLocation)
{
	this->id = App->scene->GOcounter;
	this->name =name;
	this->active = active;
	this->parent = App->scene->Root;
	filePath = FileLocation;
	App->editor->consoleApp.AddLog("Created GameObject %s \n", this->name);
}


GameObject::GameObject(const char* name,bool active, GameObject* parent, const char* FileLocation)
{
	this->id = App->scene->GOcounter;
	this->name =  name;
	this->active = active;
	this->parent = parent;
	filePath = FileLocation;
	App->editor->consoleApp.AddLog("Created GameObject %s \n", name);
}

GameObject::GameObject(GameObject* GO)
{
	this->parent = GO->parent;
	this->children = GO->children;
	this->MeshComponents = GO->MeshComponents;
	this->TransformComponents = GO->TransformComponents;
	this->MaterialComponents = GO->MaterialComponents;
	this->LightComponents = GO->LightComponents;
	App->editor->consoleApp.AddLog("Created GameObject %s \n", name);
}

GameObject::~GameObject()
{
}


void GameObject::Draw()
{
	
		
	//Draw yourself

	if (!this->MeshComponents.empty())
	{
		for (int i = 0; i != this->MeshComponents.size(); ++i)
		{
			ComponentMaterial* material = (ComponentMaterial*)this->MaterialComponents[i];

			unsigned shader = 0u;

			Texture* texture = nullptr;

			if (material != nullptr) {
				shader = material->GetShader();
				texture = material->GetTexture();
			}
			else {
				shader = App->program->programLoader;
			}

			if (texture == nullptr) {
				texture = App->textures->defaultTexture;
			}

			glUseProgram(shader);

			((ComponentMesh*)this->MeshComponents[i])->Draw(shader, texture);

		}
		
	}

	//Draw your children
	for (std::list<GameObject*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
	{
		(*it)->Draw();
	}

	glUseProgram(0);
	
}

void GameObject::DrawHierarchy()
{
	//Draw yourself
	ImGui::PushID(this);

	ImGuiTreeNodeFlags	node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (App->scene->SelectedGO == this ? ImGuiTreeNodeFlags_Selected : 0);

	if (this->children.empty()) {
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool treeOpen = ImGui::TreeNodeEx(this, node_flags, "%s %d", this->name, this->id);

	if (ImGui::IsItemClicked()) 
		App->scene->SelectedGO = this;
	
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) && ImGui::IsItemHovered())
	{
		App->editor->BeginPopup(HIERARCHY, { App->input->GetMousePosition().x,App->input->GetMousePosition().y });
	}

	if (treeOpen)
	{
		//Draw your children
		for (std::list<GameObject*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
		{
			(*it)->DrawHierarchy();
		}
		if (!(node_flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			ImGui::TreePop();
		}
	}

	ImGui::PopID();

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
		/*ImGui::SliderFloat("POS", , 0.0f, 10.0f, "%.4f", 2.0f);*/
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
		comp = new ComponentMesh();
		this->MeshComponents.push_back(comp);
		break;
	case MATERIAL:
		comp = new (ComponentMaterial);
		this->MaterialComponents.push_back(comp);
		break;
	case LIGHT:
		comp = new (ComponentLight);
		this->LightComponents.push_back(comp);
		break;
	case TRANSFORM: 	
		comp = new (ComponentTransform);
		this->TransformComponents.push_back(comp);
		break;
	}
	comp->my_go = this;
	this->components.push_back(comp);
	return comp;
}

std::string GameObject::getFileFolder()
{
	std::string s(filePath);
	std::size_t found = s.find_last_of("/\\");
	s = s.substr(0, found + 1);
	return s;
}

void GameObject::ModelTransform(unsigned shader) const 
{
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_TRUE, &GetGlobalTransform()[0][0]);
}


math::float4x4 GameObject::GetGlobalTransform() const 
{
	if (this->parent != nullptr) 
	{
		return this->parent->GetGlobalTransform() * GetLocalTransform();
	}

	return GetLocalTransform();
}

math::float4x4 GameObject::GetLocalTransform() const {
	if (transform == nullptr) 
	{
		return float4x4::identity;
	}

	return float4x4::FromTRS(transform->position, transform->rotation, transform->scale);
}