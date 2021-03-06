#include "Application.h"
#include "Globals.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "MathGeoLib.h"


bool ModuleCamera::Init()
{
	bool ret = true;
	return ret;
}

update_status ModuleCamera::PreUpdate()
{
	//rotates by calculating each frame 3 vectors and applying those to target and eye

	front = eye - target;
	front.Normalize();
	target = eye - front; //to make speed constant
	side = front.Cross(up);
	side.Normalize();
	upwards = front.Cross(side);
	upwards.Normalize();
	upwards = upwards / 5;
	side = side / 5;
	
	if(Movement)
		CameraMovement();


	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}

void ModuleCamera::CameraMovement()
{

	if (App->input->GetKey(SDL_SCANCODE_RSHIFT) || App->input->GetKey(SDL_SCANCODE_LSHIFT))
		cameraSpeed = 1.5f;
	else
		cameraSpeed = 0.5f;

	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		target = { 0,0,0 };
		eye = { 2,2,10 };
	}
	//rotates correctly but depends on distance, needs tweeking
	else if (App->input->GetKey(SDL_SCANCODE_LALT))
	{
		target = { App->scene->SelectedGO->transform->position}; //this is not optimal but I'll leave it for now
		eye += cameraSpeed * side / 5;
	}
	else if (App->input->GetMouseWheel() == WHEEL_OUT)
	{
		eye += cameraSpeed * front;
		target += cameraSpeed * front;
	}
	else if (App->input->GetMouseWheel() == WHEEL_IN)
	{
		eye -= cameraSpeed * front;
		target -= cameraSpeed * front;
	}
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		eye += cameraSpeed * up / 5;
		target += cameraSpeed * up / 5;
	}
	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		eye -= cameraSpeed * up / 5;
		target -= cameraSpeed * up / 5;
	}
	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		eye -= cameraSpeed * front;
		target -= cameraSpeed * front;
	}
	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		eye += cameraSpeed * front;
		target += cameraSpeed * front;
	}
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		eye += cameraSpeed * side;
		target += cameraSpeed * side;
	}
	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		eye -= cameraSpeed * side;
		target -= cameraSpeed * side;
	}
	Mouse.y = App->input->GetMouseMotion().y * smooth;
	Mouse.x = App->input->GetMouseMotion().x * smooth;

	//Mouse commands feel "steppy" not smooth enough but gets job done
	target += cameraSpeed * upwards * Mouse.y * mSensitivity;
	target -= cameraSpeed * side * Mouse.x* mSensitivity;

	

}

bool ModuleCamera::CleanUp()
{
	return true;
}

ModuleCamera::ModuleCamera()
{
	fbo.fbo = 0;
	fbo.fb_depth = 0;
	fbo.fb_tex = 0;
	fbo.fb_width = 0;
	fbo.fb_height = 0;
}


ModuleCamera::~ModuleCamera()
{
}
