#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_opengl2.h"
#include "SDL.h"
#include "GL/glew.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);

	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);

	if (!App->program->program)
	{
		LOG("Error: Program cannot be compiled");
		return false;
	}

	glUseProgram(App->program->program);

	//float3 vertex_buffer_data[] =
	//{
	//	{-1.0f,-1.0f,0.0f},
	//	{1.0f,-1.0f,0.0f},
	//	{0.0f,1.0f,0.0f}
	//};
	//for (int i = 0; i < 3; ++i)
	//{
	//	triangle[i] = vertex_buffer_data[i];
	//}
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update()
{
	transformationMatrix = Transform(App->camera->eye, App->camera->target);

	float4x4 Model(math::float4x4::identity); // Not moving anything

	//Triangle stuff
	//glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glVertexAttribPointer(
	//	0,                  // attribute 0
	//	3,                  // number of componentes (3 floats)
	//	GL_FLOAT,           // data type
	//	GL_FALSE,           // should be normalized?
	//	0,                  // stride
	//	(void*)0            // array buffer offset
	//);

	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "model"), 1, GL_TRUE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "view"), 1, GL_TRUE, &App->renderer->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "proj"), 1, GL_TRUE, &App->renderer->projectionMatrix[0][0]);

	for (int i = 0; i < App->modelLoader->scene->mNumMeshes; ++i) {


		unsigned vboActual = App->modelLoader->vbos[i];
		unsigned numVerticesActual = App->modelLoader->numVerticesMesh[i];
		unsigned numIndexesActual = App->modelLoader->numIndicesMesh[i];

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[App->modelLoader->textures[i]]);
		glUniform1i(glGetUniformLocation(App->program->program, "texture0"), 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboActual);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * App->modelLoader->numVerticesMesh[i]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->modelLoader->ibos[i]);
		glDrawElements(GL_TRIANGLES, numIndexesActual, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	drawGrid();

	int fragUnifLocation = glGetUniformLocation(App->program->program, "newColor");
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glUniform4fv(fragUnifLocation, 1, color);

	//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	glUseProgram(App->program->program);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	SDL_GL_DeleteContext(context);

	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
	}
	//Destroy window
	LOG("Destroying window");
	SDL_DestroyWindow(App->window->window);
	return true;
}

float4x4 ModuleRender::Transform(float3 eye, float3 target)
{
	float4x4 resultMatrix;

	float3 f(target - eye);
	f.Normalize();
	float3 s(f.Cross(App->camera->up));
	s.Normalize();
	float3 u(s.Cross(f));

	viewMatrix[0][0] = s.x; viewMatrix[0][1] = s.y; viewMatrix[0][2] = s.z; viewMatrix[3][0] = 0;
	viewMatrix[1][0] = u.x; viewMatrix[1][1] = u.y; viewMatrix[1][2] = u.z; viewMatrix[3][1] = 0;
	viewMatrix[2][0] = -f.x; viewMatrix[2][1] = -f.y; viewMatrix[2][2] = -f.z; viewMatrix[3][2] = 0;
	viewMatrix[0][3] = -s.Dot(eye); viewMatrix[1][3] = -u.Dot(eye); viewMatrix[2][3] = f.Dot(eye); viewMatrix[3][3] = 1;

	Frustum frustum;
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f)) * aspect;
	math::float4x4 proj = frustum.ProjectionMatrix();

	this->viewMatrix = viewMatrix;
	this->projectionMatrix = proj;

	resultMatrix = proj * viewMatrix;


	return resultMatrix;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height); 
}

void ModuleRender::drawGrid()
{
	//AXIS

	glLineWidth(2.0f);

	// red X
	int xAxis = glGetUniformLocation(App->program->program, "newColor");
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(xAxis, 1, red);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	glEnd();

	// green Y
	int yAxis = glGetUniformLocation(App->program->program, "newColor");
	float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(yAxis, 1, green);

	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	glEnd();

	// blue Z
	int zAxis = glGetUniformLocation(App->program->program, "newColor");
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glUniform4fv(zAxis, 1, blue);

	glBegin(GL_LINES);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();

	//GRID
	int grid = glGetUniformLocation(App->program->program, "newColor");
	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glUniform4fv(grid, 1, white);

	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

}