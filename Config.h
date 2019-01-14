#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "MathGeoLib.h"

using namespace rapidjson;

class Config
{
public:
	Config();
	~Config();

	void startObject(const char* str);
	void endObject();

	void addString(const char* str);
	void addInt(const char* str,int x);
	void addBool(const char* str, bool b);
	void addFloat(const char* str, float f);
	void addFloat3(const char* str, float3 f3);
	void addFloat4(const char* str, float4 f4);

	void addFloat4x4(const char* str, float4x4 f44);

	Document document;
	StringBuffer* string = nullptr;
	PrettyWriter<StringBuffer>* writer = nullptr;
};

#endif // __CONFIG_H_