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

	if(this->textureDiffuse!=nullptr)
		config->addInt("diffuseSelected", textureDiffuse->id);

	config->addFloat4("diffuseColor", material.diffuseColor);
	config->addFloat("diffuseK", material.diffuseK);
	config->addFloat("ambientK", material.ambientK);


	if (this->textureOcclusion != nullptr)
		config->addInt("occlusionSelected", textureOcclusion->id);


	if (this->textureSpecular != nullptr)
		config->addInt("specularSelected", textureSpecular->id);

	config->addFloat4("specularColor", material.specularColor);
	config->addFloat("specularK", material.specularK);
	config->addFloat("shininess", material.shininess);

	if (this->textureEmissive != nullptr)
		config->addInt("emissiveSelected", textureEmissive->id);

	config->addFloat4("emissiveColor", material.emissiveColor);

	config->endObject();
}

void ComponentMaterial::Load(Config* config, Value& value) 
{

	textureDiffuse->id = config->GetInt("diffuseSelected", value);
	material.diffuseColor = config->GetFloat4("diffuseColor", value);
	material.diffuseK = config->GetFloat("diffuseK", value);

	textureOcclusion->id = config->GetInt("occlusionSelected", value);
	material.ambientK = config->GetFloat("ambientK", value);

	textureSpecular->id = config->GetInt("specularSelected", value);
	material.specularColor = config->GetFloat4("specularColor", value);
	material.specularK = config->GetFloat("specularK", value);
	material.shininess = config->GetFloat("shininess", value);


	textureEmissive->id = config->GetInt("emissiveSelected", value);
	material.emissiveColor = config->GetFloat4("emissiveColor", value);
}
