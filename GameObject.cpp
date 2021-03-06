#include "GameObject.h"
#include "Component.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentLight.h"
#include "ModuleDebugDraw.h"
#include "ModuleTextures.h"
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
	this->parent = App->scene->Root;
	this->parent_UUID = App->scene->Root->UUID;
	this->name =name;
	this->active = active;
	this->CreateComponent(TRANSFORM);
	filePath = FileLocation;
	App->editor->consoleApp.AddLog("Created GameObject \n -------------\n");
}


GameObject::GameObject(const char* name,bool active, GameObject* parent, const char* FileLocation)
{
	this->id = App->scene->GOcounter;
	this->UUID = App->GenerateUUID();
	this->parent = parent;
	this->parent_UUID = parent->UUID;
	this->name =  name;
	this->active = active;
	this->CreateComponent(TRANSFORM);
	filePath = FileLocation;
	App->editor->consoleApp.AddLog("Created GameObject \n -------------\n");
}

GameObject::GameObject(const char* name, GameObject* parent) 
{
	UUID = App->GenerateUUID();

	this->name = name;

	if (parent != nullptr) {
		this->parent = parent;
		parent_UUID = parent->UUID;
		parent->children.push_back(this);
	}
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


	if(this->mesh!=nullptr)
	{
		this->BB = LoadBB();
	}
	//Check for frustum culling, could be simpler but I can track this way better
	if (this->mesh != nullptr && App->renderer->frustumCulling)
	{
		if ((App->renderer->frustum).Intersects(this->BB))
			rendered = true;
		else
			rendered = false;
	}
	else if (!App->renderer->frustumCulling)
		rendered = true;

	//actual draw. Would putting a big if(this->mesh!=nullptr) better than having two small ones?
	if (this->mesh!=nullptr && this->active && rendered)
	{
		ComponentMaterial* material = this->material;

		unsigned shader = 0u;

		Texture* texture = nullptr;

		if (material != nullptr) {
			shader = material->GetShader();
			texture = material->GetTexture(DIFFUSE);
		}
		else {
			shader = App->program->programLoader;
		}

		if (texture == nullptr) {
			texture = App->textures->defaultTexture;
		}

		glUseProgram(shader);
		ModelTransform(shader);

		this->mesh->Draw(shader, material);
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
		if(this->light!=nullptr)
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

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Ambient"))
	{
		ImGui::SliderFloat("Ambient", &this->material->material.ambientK, 0.f, 1.f);
	/*	ImGui::Image(((ImTextureID)this->material->GetTexture()->id), { size,size });*/
	}

	//Diffuse textures
	if (ImGui::CollapsingHeader("Diffuse"))
	{
		ImGui::SliderFloat("Diffuse", &this->material->material.diffuseK, 0.f, 1.f);

		if (this->material->GetTexture(DIFFUSE) != nullptr)
		{
			ImGui::Text(this->material->textureDiffuse->name);
			ImGui::Image(((ImTextureID)this->material->GetTexture(DIFFUSE)->id), { size,size });
		}
		ImGui::Text("Diffuse Texture: ");

		ImGui::PushID(0);

		if (ImGui::BeginCombo("##hidelabel", "Select diffuse Texture"))
		{
			for (std::list<Texture*>::iterator it=App->textures->textures.begin();it!=App->textures->textures.end();++it)
			{
				if (ImGui::Selectable((*it)->name))
				{
					this->material->textureDiffuse = (*it);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
	}

	//Specular Textures
	if (ImGui::CollapsingHeader("Specular"))
	{

		ImGui::SliderFloat("Specular", &this->material->material.specularK, 0.f, 1.f);
		ImGui::SliderFloat("Shininess", &this->material->material.shininess, 8.0f, 128.f);

		if (this->material->GetTexture(SPECULAR) != nullptr)
		{
			ImGui::Text(this->material->textureSpecular->name);
			ImGui::Image(((ImTextureID)this->material->GetTexture(SPECULAR)->id), { size,size });
		}

		ImGui::Text("Specular Texture: ");

		ImGui::PushID(1);

		if (ImGui::BeginCombo("##hidelabel", "Select Specular Texture"))
		{
			for (std::list<Texture*>::iterator it = App->textures->textures.begin(); it != App->textures->textures.end(); ++it)
			{
				if (ImGui::Selectable((*it)->name))
				{
					this->material->textureSpecular = (*it);
				}
			}
			ImGui::EndCombo();
		}

		ImGui::PopID();
	}

	//Emissive Textures
	if (ImGui::CollapsingHeader("Emissive"))
	{
		if (this->material->GetTexture(EMISSIVE) != nullptr)
		{
			ImGui::Text(this->material->textureEmissive->name);
			ImGui::Image(((ImTextureID)this->material->GetTexture(EMISSIVE)->id), { size,size });
		}

		ImGui::Text("Emissive Texture: ");

		ImGui::PushID(2);

		if (ImGui::BeginCombo("##hidelabel", "Select emissive Texture"))
		{
			for (std::list<Texture*>::iterator it = App->textures->textures.begin(); it != App->textures->textures.end(); ++it)
			{
				if (ImGui::Selectable((*it)->name))
				{
					this->material->textureEmissive = (*it);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
	}

	//Occlusion Textures

	if (ImGui::CollapsingHeader("Occlusion"))
	{
		if (this->material->GetTexture(OCCLUSION) != nullptr)
		{
			ImGui::Text(this->material->textureOcclusion->name);
			ImGui::Image(((ImTextureID)this->material->GetTexture(OCCLUSION)->id), { size,size });
		}

		ImGui::Text("Occlusion Texture: ");

		ImGui::PushID(3);

		if (ImGui::BeginCombo("##hidelabel", "Select occlusion Texture"))
		{
			for (std::list<Texture*>::iterator it = App->textures->textures.begin(); it != App->textures->textures.end(); ++it)
			{
				if (ImGui::Selectable((*it)->name))
				{
					this->material->textureOcclusion = (*it);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
	}

	ImGui::Separator();
}

void GameObject::DrawTransforms()
{
	int i = 0;
	
	ImGui::Text("Component Transform %i", i);
	ImGui::DragFloat3("Position",(float*)&this->transform->position, 0.1f, -100000.f, 100000.f);
	ImGui::DragFloat3("Scale", (float*)&this->transform->scale, 0.1f, 0.1f, 100.f);
	ImGui::DragFloat3("Rotation", (float*)&this->transform->eulerRot, 0.5f, -360, 360.f);
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
		comp->my_go = this;
		comp->parentUUID = this->UUID;
		this->components.push_back(comp);
		mesh = (ComponentMesh*)comp;
		break;
	case MATERIAL:
		comp = new ComponentMaterial();
		this->MaterialComponents.push_back(comp);
		comp->my_go = this;
		comp->parentUUID = this->UUID;
		this->components.push_back(comp);
		material = (ComponentMaterial*)comp;
		break;
	case LIGHT:
		comp = new (ComponentLight);
		this->LightComponents.push_back(comp);
		break;
	case TRANSFORM: 	
		comp = new (ComponentTransform);
		this->TransformComponents.push_back(comp);
		comp->my_go = this;
		comp->parentUUID = this->UUID;
		this->components.push_back(comp);
		transform = (ComponentTransform*)comp;
		break;
	}
	
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

	this->BB.Enclose(this->mesh->mesh.BB);
	
	BB.TransformAsAABB(GetGlobalTransform());

	return BB;
}

void GameObject::DrawBB()
{
	if (this->mesh!=nullptr)
	{
		// draw your BB
		dd::aabb(this->BB.minPoint, this->BB.maxPoint, float3(0.f, 1.f, 0.f), true);
	}
		// draw your children BB
	for (std::list<GameObject*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
	{
		(*it)->DrawBB();
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

void GameObject::Save(Config* config)
{
	config->StartObject();

	config->addString("uuid", UUID);
	config->addString("name", name);

	if (parent != nullptr) {
		config->addString("parentUuid", parent_UUID);
	}

	config->addBool("active", active);
	config->addBool("static", Static);

	config->StartArray("components");

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->Save(config);
	}

	config->EndArray();

	config->endObject();
}

void GameObject::Load(Config* config, Value& value) 
{

	UUID = config->GetString("uuid", value);

	active = config->GetBool("active", value);
	Static = config->GetBool("static", value);

	if (parent != nullptr) {
		parent_UUID = config->GetString("parentUuid", value);
	}
	else {
		parent_UUID = "";
	}

	rapidjson::Value components = value["components"].GetArray();

	for (Value::ValueIterator it = components.Begin(); it != components.End(); ++it) 
	{
		Component* component = CreateComponent(config->GetComponentType("componentType", (*it)));

		if (component != nullptr) {
			component->Load(config, (*it));
		}
	}

}