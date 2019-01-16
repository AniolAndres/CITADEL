#include "ComponentMesh.h"



void ComponentMesh::LoadMesh(aiMesh* AImesh)
{
	assert(AImesh != nullptr);

	mesh.verticesNumber = AImesh->mNumVertices;
	mesh.vertices = new float[mesh.verticesNumber*3];
	memcpy(mesh.vertices, AImesh->mVertices, sizeof(float) * mesh.verticesNumber * 3);

	if (AImesh->HasFaces())
	{
		mesh.indicesNumber = AImesh->mNumFaces * 3;
		mesh.indices = new unsigned[mesh.indicesNumber];
		for(unsigned i=0u;i<AImesh->mNumFaces;++i)
		{
			memcpy(&mesh.indices[i * 3], AImesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
		}
	}

	if (AImesh->HasTextureCoords(0))
	{
		mesh.uvs = new float[mesh.verticesNumber * 2];
		int uvsCounter = 0;
		for (unsigned i = 0u; i < mesh.verticesNumber; ++i)
		{
			mesh.uvs[uvsCounter] = AImesh->mTextureCoords[0][i].x;
			++uvsCounter;
			mesh.uvs[uvsCounter] = AImesh->mTextureCoords[0][i].y;
			++uvsCounter;
		}
	}

	if (AImesh->HasNormals())
	{
		mesh.normals = new float[mesh.verticesNumber * 3];
		memcpy(mesh.normals, AImesh->mNormals, sizeof(float)*mesh.verticesNumber * 3);
	}

	if (AImesh->HasVertexColors(0))
	{
		mesh.colours = new float[mesh.verticesNumber * 3];
		memcpy(mesh.colours, AImesh->mColors, sizeof(float)*mesh.verticesNumber * 3);
	}

	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);


	unsigned offset = sizeof(math::float3);

	if (mesh.normals != nullptr) {
		mesh.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	if (mesh.uvs != nullptr) {
		mesh.texturesOffset = offset;
		offset += sizeof(math::float2);
	}

	mesh.vertexSize = offset;

	glBufferData(GL_ARRAY_BUFFER, mesh.vertexSize*mesh.verticesNumber, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh.verticesNumber, mesh.vertices);

	if (mesh.normals != nullptr) 
	{
		glBufferSubData(GL_ARRAY_BUFFER, mesh.normalsOffset * mesh.verticesNumber, sizeof(float) * 3 * mesh.verticesNumber, mesh.normals);
	}

	if (mesh.uvs != nullptr) 
	{
		glBufferSubData(GL_ARRAY_BUFFER, mesh.texturesOffset * mesh.verticesNumber, sizeof(float2)*mesh.verticesNumber, mesh.uvs);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh.indicesNumber, mesh.indices, GL_STATIC_DRAW);

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	if (mesh.texturesOffset != 0) 
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texturesOffset * mesh.verticesNumber));
	}

	if (mesh.normalsOffset != 0) 
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normalsOffset * mesh.verticesNumber));
	}

	// vao off
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// vbo off
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->numIndices = AImesh->mNumFaces * 3;
	this->materialIndex = AImesh->mMaterialIndex;
	this->numVert = AImesh->mNumVertices;


	if (mesh.verticesNumber != 0)
		my_go->Static = false;
	else
		my_go->Static = true;

	mesh.BB.SetNegativeInfinity();
	mesh.BB.Enclose((float3*)mesh.vertices, mesh.verticesNumber);

	mesh.path = this->path;
}

void ComponentMesh::LoadMesh(par_shapes_mesh_s* pmesh)
{
	assert(pmesh != nullptr);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	unsigned offset = sizeof(math::float3);

	if (pmesh->normals) 
	{
		mesh.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	mesh.vertexSize = offset;

	glBufferData(GL_ARRAY_BUFFER, mesh.vertexSize * pmesh->npoints, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::float3) * pmesh->npoints, pmesh->points);

	if (pmesh->normals)
	{
		glBufferSubData(GL_ARRAY_BUFFER, mesh.normalsOffset * pmesh->npoints, sizeof(math::float3) * pmesh->npoints, pmesh->normals);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * pmesh->ntriangles * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned) * pmesh->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(pmesh->ntriangles * 3); ++i) 
	{
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

	if (mesh.texturesOffset != 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texturesOffset * mesh.verticesNumber));
	}

	if (mesh.normalsOffset != 0) 
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normalsOffset * mesh.verticesNumber));
	}

	glBindVertexArray(0);

	mesh.vertices = pmesh->points;
	mesh.verticesNumber = pmesh->npoints;

	mesh.BB.SetNegativeInfinity();
	mesh.BB.Enclose((float3*)mesh.vertices, mesh.verticesNumber);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentMesh::Draw(unsigned Program, const ComponentMaterial* mat) const 
{
	float4x4 Model = float4x4::identity;

	glUseProgram(Program);

	/*glUniform1i(glGetUniformLocation(Program, "texture0"), 0);*/

	glUniformMatrix4fv(glGetUniformLocation(Program, "view"), 1, GL_TRUE, &App->renderer->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Program, "proj"), 1, GL_TRUE, &App->renderer->projectionMatrix[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(App->program->programDefault, "view"), 1, GL_TRUE, &App->renderer->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programDefault, "proj"), 1, GL_TRUE, &App->renderer->projectionMatrix[0][0]);

	glUniform3fv(glGetUniformLocation(Program, "light_pos"), 1, (float*)&App->scene->lightPosition);

	glUniform4f(glGetUniformLocation(Program, "ambientcolor"), mat->material.ambientColor.x, mat->material.ambientColor.y, mat->material.ambientColor.z, 1.0f);

	glUniform4f(glGetUniformLocation(Program, "diffuseColor"), mat->material.diffuseColor.x, mat->material.diffuseColor.y, mat->material.diffuseColor.z, 1.0f);
	glUniform4f(glGetUniformLocation(Program, "emissiveColor"), mat->material.emissiveColor.x, mat->material.emissiveColor.y, mat->material.emissiveColor.z, 1.0f);
	glUniform4f(glGetUniformLocation(Program, "specularColor"), mat->material.specularColor.x, mat->material.specularColor.y, mat->material.specularColor.z, 1.0f);

	glUniform1f(glGetUniformLocation(Program, "ambient"), App->scene->ambientLight);
	glUniform1f(glGetUniformLocation(Program, "shininess"), mat->material.shininess);
	glUniform1f(glGetUniformLocation(Program, "k_ambient"), mat->material.ambientK);
	glUniform1f(glGetUniformLocation(Program, "k_diffuse"), mat->material.diffuseK);
	glUniform1f(glGetUniformLocation(Program, "k_specular"), mat->material.specularK);
	glUniform4fv(glGetUniformLocation(Program, "newColor"), 1, (float*)&mat->material.color);

	glActiveTexture(GL_TEXTURE0);

	if (mat->textureDiffuse!=nullptr) {
		glBindTexture(GL_TEXTURE_2D, mat->textureDiffuse->id);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, App->renderer->Fallback);
	}
	glUniform1i(glGetUniformLocation(Program, "diffuseMap"), 0);

	glActiveTexture(GL_TEXTURE1);
	if (mat->textureEmissive!=nullptr) 
	{
		glBindTexture(GL_TEXTURE_2D, mat->textureEmissive->id);
	}
	else 
	{
		glBindTexture(GL_TEXTURE_2D, App->renderer->Fallback);
	}
	glUniform1i(glGetUniformLocation(Program, "emissiveMap"), 1);

	glActiveTexture(GL_TEXTURE2);
	if (mat->textureOcclusion != nullptr) {
		glBindTexture(GL_TEXTURE_2D, mat->textureOcclusion->id);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, App->renderer->Fallback);
	}
	glUniform1i(glGetUniformLocation(Program, "occlusionMap"), 2);

	glActiveTexture(GL_TEXTURE3);
	if (mat->textureSpecular!=nullptr) {
		glBindTexture(GL_TEXTURE_2D, mat->textureSpecular->id);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, App->renderer->Fallback);
	}
	glUniform1i(glGetUniformLocation(Program, "specularMap"), 3);


	glBindVertexArray(mesh.vao);

	glDrawElements(GL_TRIANGLES, mesh.indicesNumber, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
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

void ComponentMesh::Save(Config* config)
{
	config->StartObject();

	config->addComponentType("componentType", type);
	config->addString("parent", my_go->UUID);
	config->addString("currentMesh", this->mesh.path);

	config->endObject();
}