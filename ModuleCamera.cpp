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
	//rotates by calculating each frame 3 vectors and applying those to target and eye

	front = eye - target;
	front.Normalize();
	target = eye - front; //to make speed constant
	side = front.Cross(up);
	side.Normalize();
	upwards = front.Cross(side);
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
		eye = { 2,2,10 };
	}
	//rotates correctly but depends on distance, needs tweeking
	else if (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		target = { 0,0,0 }; //this is not optimal but I'll leave it for now
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

		//Mouse commands feel "steppy" not smooth enough but gets job done,  it must be because it does not track speed, only sign

		if (App->input->GetMouseMotion().x < 0)
		{
			target += cameraSpeed * side / 3;
		}
		if (App->input->GetMouseMotion().x > 0)
		{
			target -= cameraSpeed * side / 3;
		}
		if (App->input->GetMouseMotion().y > 0)
		{
			target += cameraSpeed * upwards / 3;
		}
		if (App->input->GetMouseMotion().y < 0)
		{
			target -= cameraSpeed * upwards / 3;
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
