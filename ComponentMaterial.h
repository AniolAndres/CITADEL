#ifndef __COMPONENTMATERIAL_H_
#define __COMPONENTMATERIAL_H_

#include "Application.h"
#include "ModuleTextures.h"
#include "Component.h"
#include "Assimp/material.h"

class ComponentMaterial :
	public Component
{
public:

	void LoadMaterial(const aiMaterial* material);
	unsigned GetShader() {return shader; }
	Texture* GetTexture() { return texture; }

	void DeleteTexture();

	ComponentMaterial();
	ComponentMaterial(ComponentMaterial* Cmaterial);
	~ComponentMaterial();

private:
	unsigned shader = 0u;
	Texture* texture = nullptr;
};

#endif // __COMPONENTMATERIAL_H_