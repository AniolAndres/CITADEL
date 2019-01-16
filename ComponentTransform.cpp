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
	config->StartObject();

	config->addComponentType("componentType", type);
	config->addString("parentUuid", my_go->UUID);
	config->addString("uuid", UUID);
	config->addFloat3("position", position);
	config->addQuat("rotation", rotation);
	config->addFloat3("eulerRotation", eulerRot);
	config->addFloat3("scale", scale);

	config->endObject();
}

void ComponentTransform::Load(Config* config, Value &value)
{
	UUID = config->GetString("uuid", value);
	parentUUID, config->GetString("parentUuid", value);
	position = config->GetFloat3("position", value);
	eulerRot = config->GetFloat3("eulerRotation", value);
	scale = config->GetFloat3("scale", value);
	rotation = config->GetQuat("rotation", value);
}