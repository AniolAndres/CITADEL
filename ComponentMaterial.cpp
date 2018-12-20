#include "ComponentMaterial.h"


void ComponentMaterial::CheckMaterial(const aiMaterial* material)
{
	std::string texturePath;
	if (material != nullptr) {
		aiTextureMapping mapping = aiTextureMapping_UV;
		aiString file;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, 0);
		texturePath = my_go->getFileFolder() + file.C_Str();
	}
	else {
		texture = App->textures->defaultTexture;
	}

	DeleteTexture();
	texture = App->textures->loadImg(texturePath.c_str());
}

void ComponentMaterial::DeleteTexture()
{
	delete texture;
	texture = nullptr;
}

ComponentMaterial::ComponentMaterial()
{
	shader = App->program->programLoader;
	type = MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}
