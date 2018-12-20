#ifndef __ModuleModelLoader_H_
#define  __ModuleModelLoader_H_

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleScene.h"
#include "assimp/mesh.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "Module.h"

class Component;
class ComponentMesh;
class GameObject;

class ModuleModelLoader :public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();


	bool LoadFBX(const char* path);

	//void GenerateMeshes(const aiScene* scene);

	//void GenerateMaterials(const aiScene* scene);

	unsigned* vbos;

	unsigned* ibos;

	unsigned* materials;

	unsigned* textures;

	unsigned* numVerticesMesh;

	unsigned* numVerticesTotal;

	unsigned* numIndicesMesh;

	const aiScene* scene;

	char* modelPath;

	bool modelLoaded = false;

	ModuleModelLoader();
	~ModuleModelLoader();

	
	
};

#endif // __ModuleModelLoader_H_