#ifndef  __COMPONENTLIGHT_H_
#define  __COMPONENTLIGHT_H_ 

#include "Component.h"

enum lightType
{
	point = 1,
	directional,
	whatever
};

class ComponentLight :
	public Component
{
public:
	ComponentLight();
	~ComponentLight();

	int type;

	void SetLightType(int type);
};

#endif // __COMPONENTLIGHT_H_