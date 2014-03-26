#include "jsonparser.h"
#include "lexer.h"
#include "json.h"

JSONParser::JSONParser() :
	m_lexer (new Lexer) {}

JSONParser::~JSONParser()
{
	delete m_lexer;
}

JSONObject* JSONParser::parseJSONFile (const String& fileName)
{
	m_lexer->processFile (fileName);
	m_lexer->mustGetNext (TK_BraceStart);
	return parseObject();
}

JSONObject* JSONParser::parseObject()
{
	JSONObject* obj = new JSONObject;

	while (m_lexer->next (TK_BraceEnd) == false)
	{
		if (obj->isEmpty() == false)
			m_lexer->mustGetNext (TK_Comma);

		m_lexer->mustGetNext (TK_String);
		String propertyName = m_lexer->token()->text;

		if (obj->hasProperty (propertyName))
			error ("Attempted to re-define property '%1'", propertyName);

		m_lexer->mustGetNext (TK_Colon);
		Variant propertyValue = parseValue();

		obj->setProperty (propertyName, propertyValue);
	}

	return obj;
}

Variant JSONParser::parseValue()
{
	m_lexer->mustGetNext (TK_Any);
	String propertyValue = m_lexer->token()->text;

	switch (m_lexer->token()->type)
	{
		case TK_Symbol:
			if (propertyValue == "null")
				return Variant (null);
			elif (propertyValue == "true")
				return Variant (true);
			elif (propertyValue == "false")
				return Variant (false);
			goto badvalue;

		case TK_String:
			return propertyValue;

		case TK_Number:
			if (propertyValue.firstIndexOf (".") == -1)
				return propertyValue.toLong();

			return propertyValue.toDouble();

		case TK_BracketStart:
		{
			VariantList vlist;

			while (m_lexer->next (TK_BracketEnd) == false)
			{
				if (vlist.isEmpty() == false)
					m_lexer->mustGetNext (TK_Comma);

				vlist << parseValue();
			}

			return vlist;
		}

		case TK_BraceStart:
		{
			JSONObject* obj = parseObject();
			Variant result = obj->values();
			delete obj;
			return result;
		}

		badvalue:
		default:
			error ("bad JSON value: expected a string, number, array, object, "
				"true, false or null");
	}

	error ("");
	return Variant();
}
