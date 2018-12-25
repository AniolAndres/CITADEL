#ifndef __COMPONENTMESH_H_
#define __COMPONENTMESH_H_

#include "assert.h"
#include "Component.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "ModuleRender.h"
#include <assimp/mesh.h>
#include "Math/float3.h"
#include "GL/glew.h"

class ComponentMesh : public Component

{
public:

	void LoadMesh(aiMesh* mesh);

	void Draw(unsigned Program, const Texture* textures) const;

	std::vector<math::float3> vertices;
	const char*	name = nullptr;
	unsigned GetMaterialIndex() {return materialIndex; }

	int	numIndices = 0;
	int	materialIndex = 0;
	int numVert = 0;

	ComponentMesh();
	~ComponentMesh();

private:
	unsigned vao = 0u;
	unsigned vbo = 0u;
	unsigned ibo = 0u;
};

#endif // __COMPONENTMESH_H_