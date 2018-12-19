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

	void CheckMaterial(const aiMaterial* material);

	void DeleteTexture();

	ComponentMaterial();
	~ComponentMaterial();

private:
	unsigned shader = 0u;
	SDL_Texture* texture = nullptr;
};

#endif // __COMPONENTMATERIAL_H_