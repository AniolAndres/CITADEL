#include "Application.h"
#include "Globals.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "MathGeoLib.h"


bool ModuleCamera::Init()
{
	bool ret = true;
	return ret;
}

update_status ModuleCamera::PreUpdate()
{
	float3 front = eye - target;
	float3 dirX = { 1,0,0 };
	front.Normalize();
	target = eye - front;
	float3 side = front.Cross(up);
	side.Normalize();
	float3 upwards = front.Cross(side);
	upwards.Normalize();
	side = side / 5;
	upwards = upwards / 20;

	if (App->input->GetKey(SDL_SCANCODE_RSHIFT) || App->input->GetKey(SDL_SCANCODE_LSHIFT))
		cameraSpeed = 2.5f;
	else
		cameraSpeed = 1.0f;


	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		target = { 0,0,0 };
		eye = { 1,1,5 };
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		SDL_ShowCursor(SDL_DISABLE);
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
		//Mouse commands feel "steppy" not smooth enough but gets job done
		if (App->input->GetMouseMotion().x<0)
		{
			target += cameraSpeed * side / 5;
		}
		if (App->input->GetMouseMotion().x > 0)
		{
			target -= cameraSpeed * side / 5;
		}
		if (App->input->GetMouseMotion().y > 0)
		{
			target += cameraSpeed * upwards / 5;
		}
		if (App->input->GetMouseMotion().y < 0)
		{
			target -= cameraSpeed * upwards / 5;
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}


ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}
