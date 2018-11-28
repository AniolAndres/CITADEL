#ifndef __ModuleModelLoader_H_
#define  __ModuleModelLoader_H_


#include "assimp/mesh.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "Module.h"
class ModuleModelLoader :public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();


	bool LoadFBX(const char* path);

	void GenerateMeshes(const aiScene* scene);

	void GenerateMaterials(const aiScene* scene);

	unsigned* vbos;

	unsigned* ibos;

	unsigned* materials;

	unsigned* textures;

	unsigned* numVerticesMesh;

	unsigned* numIndicesMesh;

	const aiScene* scene;

	ModuleModelLoader();
	~ModuleModelLoader();

	
	
};

#endif // __ModuleModelLoader_H_