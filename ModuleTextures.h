#ifndef __ModuleTextures_h__
#define __ModuleTextures_h__

#include<list>
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"
#include "Math/float4.h"
#include "Geometry/AABB.h"

struct Texture
{
	int id = 0;
	int width = 0;
	int height = 0;
	const char* name = nullptr;
	Texture(int id, int width, int height, const char* name) : id(id), width(width), height(height), name(name) { }
};

struct Mesh
{
	unsigned vao = 0;
	unsigned vbo = 0;
	unsigned ibo = 0;

	float* normals = nullptr;
	float* colours = nullptr;
	float* uvs = nullptr;
	unsigned verticesNumber = 0u;
	float* vertices = nullptr;
	unsigned indicesNumber = 0u;
	unsigned* indices = nullptr;

	unsigned normalsOffset = 0u;
	unsigned texturesOffset = 0u;
	unsigned vertexSize = 0u;

	math::AABB	BB;
};


struct Material {

	unsigned occlusionMap = 0u;
	float ambientK = 0.5f;

	unsigned diffuseMap = 0u;
	float4 diffuseColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	float diffuseK = 1.0f;

	unsigned specularMap = 0u;
	float4 specularColor = {1.0f, 0.0f, 0.0f, 1.0f};
	float specularK = 0.6f;
	float shininess = 64.0f;

	unsigned emissiveMap = 0u;
	float4 emissiveColor = { 0.0f, 0.0f, 1.0f, 0.0f };

	float4	color = float4::one;
};


class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	update_status Update();
	bool CleanUp();

	Texture* loadImg(const char* path, const char* name);
	void LoadTextures();

	bool mipmaping = false;
	int filterType = GL_LINEAR;
	int mipMapMode = GL_NEAREST_MIPMAP_NEAREST;
	int wrapMode = GL_CLAMP;
	Texture* defaultTexture = nullptr;
	Texture* noCamTexture = nullptr;

	std::list<Texture*> textures;

private:
	unsigned texture0 = 0;
};

#endif // __ModuleTextures_h__