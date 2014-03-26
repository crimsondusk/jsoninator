/*
	Copyright 2014 Santeri Piippo
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
