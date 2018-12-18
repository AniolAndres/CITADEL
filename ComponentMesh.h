#ifndef __COMPONENTMESH_H_
#define __COMPONENTMESH_H_

#include "assert.h"
#include "Component.h"
#include <assimp/mesh.h>
#include "Math/float3.h"
#include "GL/glew.h"

class ComponentMesh : public Component

{
public:

	void CheckMesh(aiMesh* mesh);

	std::vector<math::float3> vertices;
	const char*	name = nullptr;
	int	numIndices = 0;
	int	materialIndex = 0;
	ComponentMesh();
	~ComponentMesh();

private:
	unsigned vao = 0u;
	unsigned vbo = 0u;
	unsigned ibo = 0u;
};

#endif // __COMPONENTMESH_H_