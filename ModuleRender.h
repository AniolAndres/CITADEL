#ifndef __ModuleRender_H_
#define __ModuleRender_H_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
struct FBO;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	float4x4 Transform(float3 eye, float3 target);

	void GenerateFBOTexture(unsigned width, unsigned height, FBO* fbo);

	void DrawDebug();

	void drawGrid();

	void* context;

	bool showTextures = true;

	bool showGrid = true;

	bool showAxis = true;

	float3 triangle[3];
	float4x4 viewMatrix = float4x4::zero;
	float4x4 projectionMatrix = float4x4::zero;
	float4x4 transformationMatrix = float4x4::zero;

	Frustum frustum;

private:
	unsigned vbo, vao = 0;


};

#endif // __ModuleRender_H_