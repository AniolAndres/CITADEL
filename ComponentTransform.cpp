#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	
	type = TRANSFORM;
}

ComponentTransform::ComponentTransform(ComponentTransform* Ctransform)
{
	this->scale = Ctransform->scale;
	this->rotation = Ctransform->rotation;
	this->position = Ctransform->position;
	type = TRANSFORM;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::generateEulerRotation()
{
	rotation = rotation.FromEulerXYZ(math::DegToRad(eulerRot.x), math::DegToRad(eulerRot.y), math::DegToRad(eulerRot.z));
}

void ComponentTransform::Save(Config* config)
{

}