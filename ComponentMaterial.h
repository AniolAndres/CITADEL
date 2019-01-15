#ifndef __COMPONENTMATERIAL_H_
#define __COMPONENTMATERIAL_H_

#include "Application.h"
#include "ModuleTextures.h"
#include "Component.h"
#include "Assimp/material.h"

enum TextureType
{
	DIFFUSE=1,
	OCCLUSION,
	SPECULAR,
	EMISSIVE,
	NORMALS
};

class ComponentMaterial :
	public Component
{
public:

	void LoadMaterial(const aiMaterial* material);
	unsigned GetShader() {return shader; }
	Texture* GetTexture(int TT);

	void DeleteTexture();

	ComponentMaterial();
	ComponentMaterial(ComponentMaterial* Cmaterial);
	~ComponentMaterial();

	Material material;
	unsigned shader = 0u;

	Texture* texture = nullptr;
	Texture* textureDiffuse = nullptr;
	Texture* textureNormals = nullptr;
	Texture* textureOcclusion = nullptr;
	Texture* textureEmissive = nullptr;
	Texture* textureSpecular = nullptr;
};

#endif // __COMPONENTMATERIAL_H_