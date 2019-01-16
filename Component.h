#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include "Globals.h"
#include "GameObject.h"
#include "Application.h"
#include "Config.h"

class GameObject;

class Component
{
public:
	Component();
	~Component();

	virtual void Enable() {}
	virtual void Disable() {}
	virtual update_status Update() 
	{
		return UPDATE_CONTINUE;
	}

	virtual void Save(Config* config) {}
	virtual void Load(Config* config, Value& value) { }

	const char* UUID = nullptr;
	const char* parentUUID = nullptr;
	int type;
	bool active;
	GameObject* my_go = nullptr;

};


#endif // __COMPONENT_H_
