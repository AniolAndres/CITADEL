#ifndef __ModuleRender_H_
#define __ModuleRender_H_

#include "Module.h"
#include "Globals.h"
#include "ImGuizmo/ImGuizmo.h"
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

	void setUniformMatrix();
	void GenerateFBOTexture(unsigned width, unsigned height, FBO* fbo);
	void GenerateFallback();
	void DrawGuizmo(float width, float height, float winPosX, float winPosY);

	void DrawDebug();

	void* context;

	bool showTextures = true;
	bool showGrid = true;
	bool showAxis = true;
	bool Vsync = false;
	bool frustumCulling = true;
	unsigned Fallback = 0u;

	float4x4 viewMatrix = float4x4::zero;
	float4x4 projectionMatrix = float4x4::zero;
	float4x4 transformationMatrix = float4x4::zero;
	math::float2 viewport = math::float2::zero;

	Frustum frustum;


	int	imGuizmoOp = 0;
	int	imGuizmoMode = 0;
private:
	unsigned vbo, vao = 0;


};

#endif // __ModuleRender_H_