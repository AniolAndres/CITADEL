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

	bool CleanUp();

	GLuint program;
};

