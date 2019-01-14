#include "GameObject.h"
#include "Component.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentLight.h"
#include "ModuleDebugDraw.h"
#include "ModuleInput.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "Geometry/Frustum.h"
#include "ComponentTransform.h"
#include "Assimp/material.h"
#include "Globals.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include "debugdraw.h"
#include<vector>
#include<list>

GameObject::GameObject()
{

}

GameObject::GameObject(const char* name, bool active, const char* FileLocation)
{
	this->id = App->scene->GOcounter;
	this->UUID = App->GenerateUUID();
	this->name =name;
	this->active = active;
	this->parent = App->scene->Root;
	this->CreateComponent(TRANSFORM);
	filePath = FileLocation;
	App->editor->consoleApp.AddLog("Created GameObject \n");
}


GameObject::GameObject(const char* name,bool active, GameObject* parent, const char* FileLocation)
{
	this->id = App->scene->GOcounter;
	this->UUID = App->GenerateUUID();
	this->name =  name;
	this->active = active;
	this->parent = parent;
	this->CreateComponent(TRANSFORM);
	filePath = FileLocation;
	App->editor->consoleApp.AddLog("Created GameObject \n");
}

GameObject::~GameObject()
{

	for(int i=0;i!=components.size();++i)	
	{
		delete components[i];
	}
	components.clear();

	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		delete (*it);
	}
	children.clear();

	transform = nullptr;
	mesh = nullptr;
	material = nullptr;
	parent = nullptr;
}


void GameObject::Draw()
{
	//Draw yourself------------------



	//Check for frustum culling, could be simpler but I can track this way better
	if (this->mesh != nullptr)
	{
		this->BB = LoadBB();

		BB.TransformAsAABB(GetGlobalTransform());
		
		if ((App->renderer->frustum).Intersects(this->BB))
			rendered = true;
		else
			rendered = false;
	}

	//actual draw. Would putting a big if(this->mesh!=nullptr) better than having two small ones?
	if (this->mesh!=nullptr && this->active && rendered)
	{

		ComponentMaterial* material = this->material;

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
		ModelTransform(shader);

		this->mesh->Draw(shader, texture);
		
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

	bool treeOpen = ImGui::TreeNodeEx(this, node_flags, "%s", this->name);

	if (ImGui::IsItemClicked())
		App->scene->SelectedGO = this;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) && ImGui::IsItemHovered())
	{
		App->scene->SelectedGO = this;
		ImGui::OpenPopup("Edit Hierarchy");
	}

	App->editor->DrawHierarchyPopup();

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
		if (this->mesh != nullptr)
			this->DrawMeshes();
		break;
	case MATERIAL:
		if (this->material != nullptr)
			this->DrawMaterials();
		break;
	case TRANSFORM:
		if (this->transform != nullptr)
		this->DrawTransforms();
		break;
	case LIGHT:
		this->DrawLights();
		break;
	}
}

void GameObject::DrawMeshes()
{
	ImGui::Text("Vertices: %d", this->mesh->mesh.verticesNumber);
}

void GameObject::DrawMaterials()
{
	float size = ImGui::GetWindowWidth();

	ImGui::Image(((ImTextureID)this->material->GetTexture()->id), { size,size });

	ImGui::Separator();

	ImGui::SliderFloat("Ambient", &this->material->material.ambientK, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &this->material->material.diffuseK, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &this->material->material.specularK, 0.f, 1.f);
	ImGui::SliderFloat("Shininess", &this->material->material.shininess, 0.f, 128.f);
}

void GameObject::DrawTransforms()
{
	int i = 0;
	
	ImGui::Text("Component Transform %i", i);
	ImGui::SliderFloat3("Position",(float*)&this->transform->position,-100.f,100.f);
	ImGui::SliderFloat3("Scale", (float*)&this->transform->scale, 0.1f, 100.f);
	ImGui::SliderFloat3("Rotation", (float*)&this->transform->eulerRot, 0.f, 360.f); //needs tweaking	
}

void GameObject::DrawLights()
{
	int i = 0;
	for (i = 0; i != this->LightComponents.size(); ++i)
	{
		ImGui::Text("Component Light #%i", i);
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
		mesh = (ComponentMesh*)comp;
		break;
	case MATERIAL:
		comp = new ComponentMaterial();
		this->MaterialComponents.push_back(comp);
		material = (ComponentMaterial*)comp;
		break;
	case LIGHT:
		comp = new (ComponentLight);
		this->LightComponents.push_back(comp);
		break;
	case TRANSFORM: 	
		comp = new (ComponentTransform);
		this->TransformComponents.push_back(comp);
		transform = (ComponentTransform*)comp;
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

math::float4x4 GameObject::GetLocalTransform() const 
{
	if (transform == nullptr) 
	{
		return float4x4::identity;
	}

	this->transform->generateEulerRotation();

	return float4x4::FromTRS(transform->position, transform->rotation, transform->scale);
}

AABB GameObject::LoadBB()
{
	BB.SetNegativeInfinity();

	this->BB.Enclose((float3*)this->mesh->mesh.vertices, this->mesh->mesh.verticesNumber);
	
	return BB;
}

void GameObject::DrawBB()
{
	if (this->mesh!=nullptr)
	{
		// draw your BB

		dd::aabb(this->BB.minPoint, this->BB.maxPoint, float3(0.f, 1.f, 0.f), true);

		// draw your children BB
		for (std::list<GameObject*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
		{
			(*it)->DrawBB();
		}
	}
}

Component* GameObject::DuplicateComponent(int type, GameObject* GO)
{
	
	Component* comp = nullptr;

	switch (type)
	{
	case MESH:
		comp = new ComponentMesh(GO->mesh);
		this->mesh = (ComponentMesh*)comp;
		break;
	case MATERIAL:
		comp = new ComponentMaterial(GO->material);
		this->material = (ComponentMaterial*)comp;
		break;
	case LIGHT:

		break;
	case TRANSFORM:
		comp = new ComponentTransform(GO->transform);
		this->transform = (ComponentTransform*)comp;
		break;
	}
	return comp;
}