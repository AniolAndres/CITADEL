#ifndef __CONFIG_H_
#define __CONFIG_H_

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
	void endObject();
	void StartArray(const char* str);
	void EndArray();

	//add variables
	void addString(const char* str);
	void addInt(const char* str,int x);
	void addBool(const char* str, bool b);
	void addFloat(const char* str, float f);
	void addFloat3(const char* str, float3 f3);
	void addFloat4(const char* str, float4 f4);
	void addUnsigned(const char* str, unsigned ui);
	void addComponentType(const char* str, int componentType);

	//Getters
	int GetComponentType(const char* name, Value& value);
	int GetInt(const char* name,Value& value);
	unsigned GetUnsigned(const char* name, Value& value);
	float GetFloat(const char* name, Value& value);
	const char* GetString(const char* name, Value& value);
	bool GetBool(const char* name, Value& value);
	float3 GetFloat3(const char* name, Value& value);
	float4 GetFloat4(const char* name, Value& value);


	void addFloat4x4(const char* str, float4x4 f44);

	Document document;
	StringBuffer* string = nullptr;
	PrettyWriter<StringBuffer>* writer = nullptr;
};

#endif // __CONFIG_H_