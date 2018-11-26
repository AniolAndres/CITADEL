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

	unsigned GenerateMeshData(const aiMesh* mesh);

	unsigned* vbos;

	const aiScene* scene;

	ModuleModelLoader();
	~ModuleModelLoader();

	
	
};

