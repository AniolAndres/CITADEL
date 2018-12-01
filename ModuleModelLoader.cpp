#include "ModuleModelLoader.h"
#include "MathGeoLib.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "GL/glew.h"


bool ModuleModelLoader::Init()
{
	bool ret = true;
	/*LoadFBX("BakerHouse.fbx");*/
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

		materials[i] = finalMaterial;
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
		ibos = new unsigned[scene->mNumMeshes];
		textures = new unsigned[scene->mNumMeshes];
		materials = new unsigned[scene->mNumMeshes];
		numVerticesMesh = new unsigned[scene->mNumMeshes];
		numIndicesMesh = new unsigned[scene->mNumMeshes];

		GenerateMeshes(scene);
		GenerateMaterials(scene);
	}
	return true;
}	

void ModuleModelLoader::GenerateMeshes(const aiScene* scene)
{

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{

		const aiMesh* sourceMesh = scene->mMeshes[i];



		glGenBuffers(1, &vbos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);


		//mVertices

		glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*sourceMesh->mNumVertices, nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * sourceMesh->mNumVertices, sourceMesh->mVertices);


		//mTexturecoords

		math::float2* textureCoords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * sourceMesh->mNumVertices,
			sizeof(float) * 2 * sourceMesh->mNumVertices, GL_MAP_WRITE_BIT);

		for (unsigned i = 0; i < sourceMesh->mNumVertices; ++i)
		{
			textureCoords[i] = math::float2(sourceMesh->mTextureCoords[0][i].x, sourceMesh->mTextureCoords[0][i].y);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//indices

		glGenBuffers(1, &ibos[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*sourceMesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

		unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
			sizeof(unsigned)*sourceMesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

		for (unsigned i = 0; i < sourceMesh->mNumFaces; ++i)
		{
			*(indices++) = sourceMesh->mFaces[i].mIndices[0];
			*(indices++) = sourceMesh->mFaces[i].mIndices[1];
			*(indices++) = sourceMesh->mFaces[i].mIndices[2];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		textures[i] = sourceMesh->mMaterialIndex;
		numVerticesMesh[i] = sourceMesh->mNumVertices;
		numIndicesMesh[i] = sourceMesh->mNumFaces * 3;
	}
}

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}
