#ifndef __ModuleProgram_H_
#define  __ModuleProgram_H_


#include "GL/glew.h"
#include "Module.h"
class ModuleProgram :	public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();

	update_status Update();
	
	char* readShaderFile(const char* shaderPath);

	void CheckCompilationErrors(GLuint shader);

	GLuint CreateProgram(const char* vertPath, const char* fragPath);

	bool CleanUp();

	GLuint programLoader;

	GLuint programDefault;

	GLuint programBlinn;
};

#endif // __ModuleProgram_H_