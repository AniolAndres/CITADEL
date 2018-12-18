#include "ComponentMesh.h"


void ComponentMesh::CheckMesh(aiMesh* mesh)
{
	assert(mesh != nullptr);

	// To be able to render in imgui we need a vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Positions
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mesh->mNumVertices * 5, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * mesh->mNumVertices, mesh->mVertices);

	// Texture coords
	float* textureBuffer = (float*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, sizeof(float) * 2 * mesh->mNumVertices, GL_MAP_WRITE_BIT);
	for (unsigned i = 0u; i < mesh->mNumVertices; ++i) {
		*(textureBuffer++) = mesh->mTextureCoords[0][i].x;
		*(textureBuffer++) = mesh->mTextureCoords[0][i].y;

		vertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Indexes
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

	int* indices = (int*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned) * mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);
	for (unsigned i = 0u; i < mesh->mNumFaces; ++i) {
		assert(mesh->mFaces[i].mNumIndices == 3);

		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * mesh->mNumVertices));

	// vao off
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// vbo off
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//bbox.SetNegativeInfinity();
	//bbox.Enclose((float3*)mesh->mVertices, mesh->mNumVertices);

	numIndices = mesh->mNumFaces * 3;
	materialIndex = mesh->mMaterialIndex;
}

ComponentMesh::ComponentMesh()
{
	type = MESH;
}


ComponentMesh::~ComponentMesh()
{
}
