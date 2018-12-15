#include "ComponentLight.h"

void ComponentLight::SetLightType(int t)
{
	this->LightType = t;
}

ComponentLight::ComponentLight()
{
	type = LIGHT;
}


ComponentLight::~ComponentLight()
{
}
