#include "ComponentMaterial.h"


void ComponentMaterial::LoadMaterial(const aiMaterial* material)
{
	std::string texturePath;
	if (material != nullptr) 
	{
		aiTextureMapping mapping = aiTextureMapping_UV;
		aiString file;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, 0);
		texturePath = my_go->getFileFolder() + file.C_Str();
		DeleteTexture();
	/*	texture = App->textures->loadImg(texturePath.c_str());*/
	}
	else 
	{
		texture = App->textures->defaultTexture;
	}
}

Texture* ComponentMaterial::GetTexture(int TextureType)
{
	switch (TextureType)
	{
	case DIFFUSE:
		return textureDiffuse;
		break;
	case OCCLUSION:
		return textureOcclusion;
		break;
	case SPECULAR:
		return textureSpecular;
		break;
	case NORMALS:
		return textureNormals;
		break;
	case EMISSIVE:
		return textureEmissive;
		break;
	}
}

void ComponentMaterial::DeleteTexture()
{
	delete texture;
	texture = nullptr;
}

ComponentMaterial::ComponentMaterial()
{
	shader = App->program->programBlinnPhong;
	type = MATERIAL;
}

ComponentMaterial::ComponentMaterial(ComponentMaterial* Cmaterial)
{
	this->shader = Cmaterial->shader;
	this->texture = Cmaterial->texture;
	type = MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Save(Config* config)
{
	config->StartObject();

	config->addComponentType("componentType", type);
	config->addString("parent", my_go->UUID);

	config->addString("diffuseSelected", textureDiffuse->path);
	config->addFloat4("diffuseColor", material.diffuseColor);
	config->addFloat("diffuseK", material.diffuseK);

	config->addString("occlusionSelected", textureOcclusion->path);
	config->addFloat("ambientK", material.ambientK);

	config->addString("specularSelected", textureSpecular->path);
	config->addFloat4("specularColor", material.specularColor);
	config->addFloat("specularK", material.specularK);
	config->addFloat("shininess", material.shininess);

	config->addString("emissiveSelected", textureEmissive->path);
	config->addFloat4("emissiveColor", material.emissiveColor);

	config->endObject();
}