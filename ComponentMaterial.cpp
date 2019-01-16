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
