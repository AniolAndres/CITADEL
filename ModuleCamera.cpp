#include "Application.h"
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

	if (App->input->GetKey(SDL_SCANCODE_DOWN))
	{
		target += upwards;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP))
	{
		target -= upwards;
	}
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		eye += up / 5;
		target += up / 5;
	}
	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		eye -= up / 5;
		target -= up / 5;
	}
	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		eye -= front;
		target -= front;
	}
	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		eye += front;
		target += front;
	}
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		eye += side;
		target += side;
	}
	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		eye -= side;
		target -= side;
	}
	//Accelration isn't adequate but gets job done, missing some tweeking, angle checking



	//triangle disappears when looked from behind??

	if (App->input->GetKey(SDL_SCANCODE_LEFT))
	{
		target += side;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
	{
		target -= side;
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
