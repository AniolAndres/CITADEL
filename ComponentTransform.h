#ifndef __COMPONENTTRANSFORM_H_
#define __COMPONENTTRANSFORM_H_

#include "Component.h"
#include "MathGeoLib.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	float3 position, scale;
	Quat rotation;

};

#endif // __COMPONENTTRANSFORM_H_