#include "Config.h"



Config::Config()
{
	string = new StringBuffer();
	writer = new PrettyWriter<StringBuffer>();
	writer->StartObject();
}


Config::~Config()
{
}

void Config::StartObject(const char* str)
{
	writer->String(str);
	writer->StartObject();
}

void Config::StartObject() 
{
	writer->StartObject();
}

void Config::endObject()
{
	writer->EndObject();
}

void Config::addString(const char* str, const char* value)
{
	writer->String(str);
	writer->String(value);
}

void Config::addInt(const char* str, int x)
{
	writer->String(str);
	writer->Int(x);
}

void Config::addBool(const char* str, bool b)
{
	writer->String(str);
	writer->Bool(b);
}

void Config::addFloat(const char* str, float f)
{
	writer->String(str);
	writer->Double(f);
}

void Config::addFloat3(const char* str, float3 f3)
{
	//hard writing better/faster than a for loop
	StartObject(str);
	writer->String("x");
	writer->Double(f3.x);
	writer->String("y");
	writer->Double(f3.y);
	writer->String("z");
	writer->Double(f3.z);
	writer->EndObject();
}

void Config::addFloat4(const char* str, float4 f4)
{
	StartObject(str);
	writer->String("x");
	writer->Double(f4.x);
	writer->String("y");
	writer->Double(f4.y);
	writer->String("z");
	writer->Double(f4.z);
	writer->String("w");
	writer->Double(f4.w);
	endObject();
}


void Config::addUnsigned(const char* str, unsigned ui)
{
	writer->String(str);
	writer->Uint(ui);
}

void Config::addComponentType(const char* str, int ComponentType)
{
	switch (ComponentType)
	{
	case MESH:
		writer->String("Mesh");
		break;
	case MATERIAL:
		writer->String("Material");
		break;
	case TRANSFORM:
		writer->String("Transform");
		break;
	case LIGHT:
		writer->String("Light");
		break;
	}
}

int Config::GetComponentType(const char* str, Value &value)
{
	const char* ComponentType = value[str].GetString();

	if (ComponentType == "Mesh")
		return MESH;
	if (ComponentType == "Material")
		return MATERIAL;
	if (ComponentType == "Transform")
		return TRANSFORM;
	if (ComponentType == "Light")
		return LIGHT;
}

int Config::GetInt(const char* str, Value &value)
{
	return value[str].GetInt();
}

unsigned Config::GetUnsigned(const char* str, Value &value)
{
	return value[str].GetUint();
}

float Config::GetFloat(const char* str, Value &value)
{
	return value[str].GetFloat();
}

const char* Config::GetString(const char* str, Value &value)
{
	return value[str].GetString();
}

bool Config::GetBool(const char* str, Value &value)
{
	return value[str].GetBool();
}

float3 Config::GetFloat3(const char* str, Value &value)
{
	float3 result = {
		value[str]["x"].GetFloat(),
		value[str]["y"].GetFloat(),
		value[str]["z"].GetFloat()
	};

	return result;
}

float4 Config::GetFloat4(const char* str, Value &value)
{
	float4 result = {
		value[str]["x"].GetFloat(),
		value[str]["y"].GetFloat(),
		value[str]["z"].GetFloat(),
		value[str]["w"].GetFloat()
	};

	return result;
}

Quat Config::GetQuat(const char* str, Value &value)
{
	Quat result = {
	value[str]["x"].GetFloat(),
	value[str]["y"].GetFloat(),
	value[str]["z"].GetFloat(),
	value[str]["w"].GetFloat()
	};

	return result;
}

void Config::addQuat(const char* str, Quat quat)
{
	StartObject(str);
	writer->String("x");
	writer->Double(quat.x);
	writer->String("y");
	writer->Double(quat.y);
	writer->String("z");
	writer->Double(quat.z);
	writer->String("w");
	writer->Double(quat.w);
	endObject();
}
//
//void StartArray(const char* name);
//void EndArray();

void Config::StartArray(const char* str)
{
	writer->String(str);
	writer->StartArray();
}

void Config::EndArray()
{
	writer->EndArray();
}