#include "ComponentMesh.h"



void ComponentMesh::LoadMesh(aiMesh* AImesh)
{
	assert(AImesh != nullptr);

	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	// mVertices

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*AImesh->mNumVertices * 5, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * AImesh->mNumVertices, AImesh->mVertices);

	//mTexturecoords

	float2* textureCoords = (float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * AImesh->mNumVertices, sizeof(float) * 2 * AImesh->mNumVertices, GL_MAP_WRITE_BIT);
	for (unsigned i = 0u; i < AImesh->mNumVertices; ++i) {
		textureCoords[i] = math::float2(AImesh->mTextureCoords[0][i].x, AImesh->mTextureCoords[0][i].y);
	/*	vertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);*/
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Indexes
	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * AImesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

	int* indices = (int*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned) * AImesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0u; i < AImesh->mNumFaces; ++i) {
		assert(AImesh->mFaces[i].mNumIndices == 3);

		*(indices++) = AImesh->mFaces[i].mIndices[0];
		*(indices++) = AImesh->mFaces[i].mIndices[1];
		*(indices++) =AImesh->mFaces[i].mIndices[2];

	/*	vertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);*/
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * AImesh->mNumVertices));

	// vao off
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// vbo off
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->numIndices = AImesh->mNumFaces * 3;
	this->materialIndex = AImesh->mMaterialIndex;
	this->numVert = AImesh->mNumVertices;


	if (this->numVert != 0)
		my_go->Static = false;
	else
		my_go->Static = true;

	BB.SetNegativeInfinity();
	BB.Enclose((float3*)AImesh->mVertices, this->numVert);
}

void ComponentMesh::LoadMesh(par_shapes_mesh_s* pmesh)
{
	assert(pmesh != nullptr);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	unsigned offset = sizeof(math::float3);

	if (pmesh->normals) {
		mesh.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	mesh.vertexSize = offset;

	glBufferData(GL_ARRAY_BUFFER, mesh.vertexSize * pmesh->npoints, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::float3) * pmesh->npoints, pmesh->points);

	if (pmesh->normals) {
		glBufferSubData(GL_ARRAY_BUFFER, mesh.normalsOffset * pmesh->npoints, sizeof(math::float3) * pmesh->npoints, pmesh->normals);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * pmesh->ntriangles * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned) * pmesh->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(pmesh->ntriangles * 3); ++i) {
		*(indices++) = pmesh->triangles[i];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mesh.verticesNumber = pmesh->npoints;
	mesh.indicesNumber = pmesh->ntriangles * 3;

	glGenVertexArrays(1, &mesh.vao);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (mesh.normalsOffset != 0) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normalsOffset * mesh.verticesNumber));
	}

	if (mesh.texturesOffset != 0) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texturesOffset * mesh.verticesNumber));
	}

	glBindVertexArray(0);

	mesh.BB.SetNegativeInfinity();
	mesh.BB.Enclose((float3*)pmesh->points, pmesh->npoints);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentMesh::Draw(unsigned Program, const Texture* texture) const 
{
	glUseProgram(Program);

	glActiveTexture(GL_TEXTURE0);

	if (texture != nullptr) 
	{
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}

	glUniform1i(glGetUniformLocation(Program, "texture0"), 0);


	glUniformMatrix4fv(glGetUniformLocation(Program, "view"), 1, GL_TRUE, &App->renderer->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Program, "proj"), 1, GL_TRUE, &App->renderer->projectionMatrix[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(App->program->programNoTextures, "view"), 1, GL_TRUE, &App->renderer->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programNoTextures, "proj"), 1, GL_TRUE, &App->renderer->projectionMatrix[0][0]);

	glBindVertexArray(mesh.vao);

	glDrawElements(GL_TRIANGLES, mesh.indicesNumber, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void ComponentMesh::EditorMeshes()
{
	ImGui::Text("Vertices: %d", this->numVert);
}

ComponentMesh::ComponentMesh()
{
	type = MESH;
}

ComponentMesh::ComponentMesh(ComponentMesh* Cmesh)
{
	type = MESH;
}

ComponentMesh::~ComponentMesh()
{
	if (mesh.vbo != 0) {
		glDeleteBuffers(1, &mesh.vbo);
	}

	if (mesh.ibo != 0) {
		glDeleteBuffers(1, &mesh.ibo);
	}
}
