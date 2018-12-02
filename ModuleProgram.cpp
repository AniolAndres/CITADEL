#include "ModuleProgram.h"
#include <errno.h>
#include "GL/glew.h"


ModuleProgram::ModuleProgram()
{
}


ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init()
{
	bool ret = true;
	
	programLoader = CreateProgram("./vertModelLoader.txt" , "./fragModelLoader.txt");

	programGrid = CreateProgram("./vertGrid.txt", "./fragGrid.txt");

	programNoTextures = CreateProgram("./vertNoTextures.txt", "./fragNoTextures.txt");

	return ret;
}

char* ModuleProgram::readShaderFile(const char* shaderPath)
{
	FILE* file = nullptr;
	errno_t  err = fopen_s(&file, shaderPath, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		char* data = (char*)malloc(size + 1);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
		return data;
	}

	return nullptr;
}

update_status ModuleProgram::Update()
{


	return UPDATE_CONTINUE;
}

void ModuleProgram::CheckCompilationErrors(GLuint shader) {
	GLint infoLogLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	GLchar* strInfoLog = new GLchar[infoLogLength + 1];
	glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

	LOG(strInfoLog);

	delete[] strInfoLog;
	infoLogLength = NULL;
	glDeleteShader(shader); // Don't leak the shader.
}

bool ModuleProgram::CleanUp()
{
	bool ret=true;
	return ret;
}

GLuint ModuleProgram::CreateProgram(const char* vertPath, const char* fragPath)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint compileStatus = GL_FALSE;
	int logLength = 0;



	char* vertShaderStr = readShaderFile(vertPath);
	char* fragShaderStr = readShaderFile(fragPath);

	if (vertShaderStr == nullptr || fragShaderStr == nullptr) {
		LOG("Error: Reading shaders failed");
		return GL_FALSE;
	}

	// Compile shaders
	glShaderSource(vertShader, 1, &vertShaderStr, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {
		LOG("Error: Failed compiling vertex shader");
		CheckCompilationErrors(vertShader);
		return GL_FALSE;
	}

	glShaderSource(fragShader, 1, &fragShaderStr, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {
		LOG("Error: Failed compiling fragment shader");
		CheckCompilationErrors(fragShader);
		return GL_FALSE;
	}

	// Compile Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &compileStatus);

	// Remove shaders, we wont need them anymore if they are loaded correctly into Program
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}