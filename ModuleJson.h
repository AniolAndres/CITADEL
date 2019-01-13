#ifndef __MODULEJSON_H_
#define __MODULEJSON_H_

#include "Module.h"
#include "Application.h"
#include "Config.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

class ModuleJson :	public Module
{
public:
	ModuleJson();
	~ModuleJson();

	rapidjson::Document document;

	bool Init();
};

#endif // __MODULEJSON_H_
