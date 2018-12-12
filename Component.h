#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include "Globals.h"
#include "GameObject.h"

class GameObject;

class Component
{
public:
	Component();
	~Component();

	virtual void Enable() {}
	virtual void Disable() {}
	virtual update_status Update() {
		return UPDATE_CONTINUE;
	}


	unsigned UID;
	int type;
	bool active;
	GameObject* my_go;
};


#endif // __COMPONENT_H_
