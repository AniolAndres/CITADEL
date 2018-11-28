#include "ModuleModelLoader.h"
#include "Application.h"
#include "GL/glew.h"


unsigned ModuleModelLoader::GenerateMeshData(const aiMesh* mesh)
{

	unsigned vbo = 0;

	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	////glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 5 * sizeof(float), NULL, GL_STREAM_DRAW);
	////glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);
	////glBufferSubData(GL_ARRAY_BUFFER,  mesh->mNumVertices * 3 * sizeof(float), mesh->mNumVertices * 2 * sizeof(float), &mesh->mTextureCoords[0]);

	//glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), NULL, GL_STREAM_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	return vbo;
}



bool ModuleModelLoader::Init()
{
	bool ret = true;
	return ret;
}

update_status ModuleModelLoader::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleModelLoader::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleModelLoader::GenerateMeshes(const aiScene* scene)
{

}

void ModuleModelLoader::GenerateMaterials(const aiScene* scene)
{

}

bool ModuleModelLoader::LoadFBX(const char* path)
{
	const char* err;

	scene = aiImportFile("BakerHouse.fbx", 0);
	LOG("Loading Scene")

	if (scene == nullptr)
	{
		err = aiGetErrorString();
		LOG(err);
	}
	else
	{
		vbos = new unsigned[scene->mNumMeshes];
	}
	GenerateMeshes(scene);
	GenerateMaterials(scene);

	return true;
}	

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}
