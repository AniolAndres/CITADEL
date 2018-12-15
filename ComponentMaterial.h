#ifndef __COMPONENTMATERIAL_H_
#define __COMPONENTMATERIAL_H_


#include "Component.h"
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();
	int type = MATERIAL;
};

#endif // __COMPONENTMATERIAL_H_