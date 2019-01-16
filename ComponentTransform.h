#ifndef __COMPONENTTRANSFORM_H_
#define __COMPONENTTRANSFORM_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "Math/float4x4.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "Config.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	ComponentTransform(ComponentTransform* Ctransform);
	~ComponentTransform();

	void Save(Config* config) override;
	void Load(Config* config, Value &value) override;

	void generateEulerRotation();

	float3 position = float3::zero;
	float3 scale = float3::zero;
	float3 eulerRot = float3::zero;
	Quat rotation = Quat::identity;

};

#endif // __COMPONENTTRANSFORM_H_