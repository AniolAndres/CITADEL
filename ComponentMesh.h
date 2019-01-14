#ifndef __COMPONENTMESH_H_
#define __COMPONENTMESH_H_

#include "assert.h"
#include "Component.h"
#include "ModuleTextures.h"
#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleRender.h"
#include <assimp/mesh.h>
#include "IMGUI/imgui.h"
#include "Math/float3.h"
#include "Geometry/AABB.h"
#include "GL/glew.h"
#include "par_shapes.h"

class ComponentMesh : public Component

{
public:

	void LoadMesh(aiMesh* mesh);
	void LoadMesh(par_shapes_mesh_s* pmesh);

	void Draw(unsigned Program, const ComponentMaterial* mat) const;

	void EditorMeshes();

	std::vector<math::float3> vertices;
	const char*	name = nullptr;
	unsigned GetMaterialIndex() {return materialIndex; }

	char* path;
	int	numIndices = 0;
	int	materialIndex = 0;
	int numVert = 0;
	//AABB BB;
	Mesh mesh;


	ComponentMesh();
	ComponentMesh(ComponentMesh* Cmesh);
	~ComponentMesh();

private:
	unsigned vao = 0u;
	unsigned vbo = 0u;
	unsigned ibo = 0u;
};

#endif // __COMPONENTMESH_H_