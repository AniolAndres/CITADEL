#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include "debugdraw.h"
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

	if (context != nullptr)
	{
		App->editor->consoleApp.AddLog("SDL context succesfully created \n");
	}
	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glViewport(0, 0, App->window->windowWidth, App->window->windowHeight);

	if (!App->program->programLoader || !App->program->programBlinn || !App->program->programDefault)
	{
		App->editor->consoleApp.AddLog("Program could not be compiled \n");
		return false;
	}

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

	//setUniformMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, App->camera->fbo.fbo);
	glViewport(0, 0, App->camera->fbo.fb_width, App->camera->fbo.fb_height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GenerateFBOTexture(App->editor->drawWidth, App->editor->drawHeight, &(App->camera->fbo));

	App->scene->Draw();

	DrawDebug();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{



	glUseProgram(0);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
	float aspect = App->editor->drawWidth/ App->editor->drawHeight;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = eye;
	frustum.front = f;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 300.f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f)) * aspect;
	math::float4x4 proj = frustum.ProjectionMatrix();
	/*math::float4x4 viewMatrix = frustum.ViewMatrix();*/

	this->viewMatrix = viewMatrix;
	this->projectionMatrix = proj;

	resultMatrix = proj * viewMatrix;

	this->frustum = frustum;

	return resultMatrix;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height); 
	App->window->windowHeight = height;
	App->window->windowWidth = width;
}

	//void ModuleRender::drawGrid()
//{
//	//AXIS
//
//	glLineWidth(2.0f);
//
//	float4x4 Model(math::float4x4::identity); // Not moving anything
//
//	glUseProgram(App->program->programGrid);
//
//	glUniformMatrix4fv(glGetUniformLocation(App->program->programGrid, "model"), 1, GL_TRUE, &Model[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(App->program->programGrid, "view"), 1, GL_TRUE, &App->renderer->viewMatrix[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(App->program->programGrid, "proj"), 1, GL_TRUE, &App->renderer->projectionMatrix[0][0]);
//
//	int fragUnifLocation = glGetUniformLocation(App->program->programGrid, "newColor");
//	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	glUniform4fv(fragUnifLocation, 1, color);
//
//	// red X
//	int xAxis = glGetUniformLocation(App->program->programGrid, "newColor");
//	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
//	glUniform4fv(xAxis, 1, red);
//
//	glBegin(GL_LINES);
//	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
//	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
//	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
//	glEnd();
//
//	// green Y
//	int yAxis = glGetUniformLocation(App->program->programGrid, "newColor");
//	float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
//	glUniform4fv(yAxis, 1, green);
//
//	glBegin(GL_LINES);
//	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
//	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
//	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
//	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
//	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
//	glEnd();
//
//	// blue Z
//	int zAxis = glGetUniformLocation(App->program->programGrid, "newColor");
//	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
//	glUniform4fv(zAxis, 1, blue);
//
//	glBegin(GL_LINES);
//	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
//	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
//	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
//	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
//	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
//	glEnd();
//
//	//GRID
//	int grid = glGetUniformLocation(App->program->programGrid, "newColor");
//	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
//	glUniform4fv(grid, 1, white);
//
//	glLineWidth(1.0f);
//	float d = 200.0f;
//	glBegin(GL_LINES);
//	for (float i = -d; i <= d; i += 1.0f)
//	{
//		glVertex3f(i, 0.0f, -d);
//		glVertex3f(i, 0.0f, d);
//		glVertex3f(-d, 0.0f, i);
//		glVertex3f(d, 0.0f, i);
//	}
//	glEnd();
//	glUseProgram(App->program->programLoader);
//}

void ModuleRender::GenerateFBOTexture(unsigned width, unsigned height, FBO* fbo)
{
	if (width != fbo->fb_width || height != fbo->fb_height)
	{
		if (fbo->fb_tex != 0)
		{
			glDeleteTextures(1, &(fbo->fb_tex));
		}

		if (width != 0 && height != 0)
		{
			if (fbo->fbo == 0)
			{
				glGenFramebuffers(1, &(fbo->fbo));
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);
			glGenTextures(1, &(fbo->fb_tex));
			glBindTexture(GL_TEXTURE_2D,fbo->fb_tex);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glGenRenderbuffers(1, &(fbo->fb_depth));
			glBindRenderbuffer(GL_RENDERBUFFER, fbo->fb_depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->fb_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->fb_tex, 0);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		fbo->fb_width = width;
		fbo->fb_height = height;
	}

}

void ModuleRender::DrawDebug()
{
	if (showGrid)
		dd::xzSquareGrid(-1000.0f, 1000.0f, 0.0f, 1.0f, math::float3(0.65f, 0.65f, 0.65f));

	if (showAxis) 
		dd::axisTriad(math::float4x4::identity, 0.1f, 1.0f, 0, true);

	if (App->scene->SelectedGO && App->scene->SelectedGO->active)
		App->scene->SelectedGO->DrawBB();

	App->debugDraw->Draw(&App->camera->fbo);
}

void ModuleRender::setUniformMatrix()
{
	float4x4 Model(math::float4x4::identity);

	//could be done iterating
	glUniformMatrix4fv(glGetUniformLocation(App->program->programBlinn, "model"), 1, GL_TRUE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programBlinn, "view"), 1, GL_TRUE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programBlinn, "proj"), 1, GL_TRUE, &projectionMatrix[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(App->program->programLoader, "model"), 1, GL_TRUE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programLoader, "view"), 1, GL_TRUE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programLoader, "proj"), 1, GL_TRUE, &projectionMatrix[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(App->program->programDefault, "model"), 1, GL_TRUE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programDefault, "view"), 1, GL_TRUE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->programDefault, "proj"), 1, GL_TRUE, &projectionMatrix[0][0]);

	
}


void ModuleRender::GenerateFallback()
{
	char fallbackImage[3] = { GLubyte(255), GLubyte(255), GLubyte(255) };
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &Fallback);
	glBindTexture(GL_TEXTURE_2D, Fallback);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, fallbackImage);
}
