#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	rotation = Quat::FromEulerXYZ(0.f, 0.f, 0.f);
	type = TRANSFORM;
}


ComponentTransform::~ComponentTransform()
{
}
