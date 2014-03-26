#pragma once
#include "main.h"

class JSONObject;
class Lexer;

class JSONParser
{
public:
	JSONParser();
	~JSONParser();

	JSONObject*	parseJSONFile (const String& fileName);

private:
	Lexer*		m_lexer;

	JSONObject*	parseObject();
	Variant		parseValue();
};
