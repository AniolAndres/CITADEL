#ifndef __ModuleCamera_H_
#define __ModuleCamera_H_


#include "Module.h"
#include "Application.h"
#include "MathGeoLib.h"


struct FBO {
	unsigned fbo;
	unsigned fb_depth;
	unsigned fb_tex;
	unsigned fb_width;
	unsigned fb_height;
};


class ModuleCamera : public Module
{
public:


	bool Init();
	update_status PreUpdate();
	update_status Update();

	float3 target = { 0,0,0 };
	float3 eye = { 2,2,10 };
	float3 up = { 0,1,0 };
	float3 front,side,upwards;
	float cameraSpeed = 1;


	FBO fbo;

	ModuleCamera();
	~ModuleCamera();

private:

};

#endif // __ModuleCamera_H_


