
#include "Module.h"
class ModuleModelLoader :
	public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	unsigned GenerateMeshData(const aiMesh* mesh);

	ModuleModelLoader();
	~ModuleModelLoader();

private:
	const aiScene* scene;
};

