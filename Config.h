#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "Application.h"
#include "ModuleFileSystem.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "Globals.h"
#include "MathGeoLib.h"


using namespace rapidjson;

class Config
{
public:
	Config();
	~Config();

	void StartObject(const char* str);
	void StartObject();
	void endObject();
	void StartArray(const char* str);
	void EndArray();

	//add variables
	void addString(const char* str, const char* value);
	void addInt(const char* str,int x);
	void addBool(const char* str, bool b);
	void addFloat(const char* str, float f);
	void addFloat3(const char* str, float3 f3);
	void addFloat4(const char* str, float4 f4);
	void addUnsigned(const char* str, unsigned ui);
	void addComponentType(const char* str, int componentType);
	void addQuat(const char* str, Quat quat);

	//Getters
	int GetComponentType(const char* str, Value& value);
	int GetInt(const char* str,Value& value);
	unsigned GetUnsigned(const char* str, Value& value);
	float GetFloat(const char* str, Value& value);
	const char* GetString(const char* str, Value& value);
	bool GetBool(const char* str, Value& value);
	float3 GetFloat3(const char* str, Value& value);
	float4 GetFloat4(const char* str, Value& value);
	Quat GetQuat(const char* str, Value& value);

	void WriteToDisk();
	Document LoadFromDisk();

	StringBuffer* string = nullptr;
	PrettyWriter<StringBuffer>* writer = nullptr;
};

#endif // __CONFIG_H_