#ifndef __MODULEDEBUGDRAW_H_
#define __MODULEDEBUGDRAW_H_

#include "Module.h"
#include "ModuleCamera.h"
#include "GL/glew.h"

class DDRenderInterfaceCoreGL;

class ModuleDebugDraw :
	public Module
{
public:
	
	bool Init();
	bool CleanUp();
	update_status Update();

	void Draw(FBO* fbo);

	ModuleDebugDraw();
	~ModuleDebugDraw();

private:
	static DDRenderInterfaceCoreGL* implementation;
};

#endif // __MODULEDEBUGDRAW_H_
