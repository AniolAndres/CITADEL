#include "ModuleModelLoader.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "GL/glew.h"


unsigned ModuleModelLoader::GenerateMeshData(const aiMesh* mesh)
{

	unsigned vbo = 0;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	return vbo;
}



bool ModuleModelLoader::Init()
{
	bool ret = true;
	LoadFBX("BakerHouse.fbx");
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
//
//void ModuleModelLoader::GenerateMeshes(const aiScene* scene)
//{

//
//
//
//}
//
void ModuleModelLoader::GenerateMaterials(const aiScene* scene)
{
	const aiMaterial* sourceMaterial;

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{

		unsigned finalMaterial;
		unsigned UVindex;
		aiString file;
		aiTextureMapping map;

		sourceMaterial = scene->mMaterials[i];

		if (sourceMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file, &map, &UVindex) == AI_SUCCESS)
		{
			finalMaterial = App->textures->loadImg(file.C_Str());
		}

		sourceMaterial = scene->mMaterials[i];


	}
}

bool ModuleModelLoader::LoadFBX(const char* path)
{
	const char* err;

	scene = aiImportFile(path, 0);
	LOG("Loading Scene")


	if (scene == nullptr)
	{
		err = aiGetErrorString();
		LOG(err);
	}
	else
	{

		vbos = new unsigned[scene->mNumMeshes];

		for (unsigned i = 0; i < scene->mNumMeshes; ++i)
		{
			vbos[i] = GenerateMeshData(scene->mMeshes[i]);
		}
		GenerateMaterials(scene);
	}
	return true;
}	

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}
