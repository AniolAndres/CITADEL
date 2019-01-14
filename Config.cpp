#include "Config.h"



Config::Config()
{
	string = new StringBuffer();
	writer = new PrettyWriter<StringBuffer>();
}


Config::~Config()
{
}

void Config::startObject(const char* str)
{
	writer->String(str);
	writer->StartObject();
}

void Config::endObject()
{
	writer->EndObject();
}

void Config::addString(const char* str)
{
	writer->String(str);
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
	startObject(str);
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
	startObject(str);
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




