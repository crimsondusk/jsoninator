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

#include <stdio.h>
#include "json.h"
#include "jsonparser.h"

static String	g_loadError;

JSONObject::JSONObject() {}
JSONObject::~JSONObject() {}

const Variant& JSONObject::property (const String& name)
{
	auto it = m_values.find (name);

	if (it == m_values.end())
		throw std::runtime_error (format ("No JSON property with key '%1' found.", name));

	return it->second;
}

void JSONObject::setProperty (const String& name, const Variant& value)
{
	auto it = m_values.find (name);

	if (it != m_values.end())
		m_values.erase (it);

	m_values.emplace (name, value);
}

const VariantMap& JSONObject::values() const
{
	return m_values;
}

bool JSONObject::isEmpty() const
{
	return m_values.size() == 0;
}

bool JSONObject::hasProperty (const String& name)
{
	return m_values.find (name) != m_values.end();
}

JSONObject* JSONObject::loadFromFile (const String& name)
{
	try
	{
		JSONParser parser;
		return parser.parseJSONFile (name);
	}
	catch (std::exception& e)
	{
		g_loadError = e.what();
		return null;
	}
}

String JSONObject::loadError()
{
	return g_loadError;
}

bool JSONObject::saveToFile (const String& fileName)
{
	try
	{
		FILE* fp = fopen (fileName, "w");

		if (fp == null)
			return false;

		printTo (fp, valueToString (values(), 0));
		fclose (fp);
		return true;
	}
	catch (std::exception& e)
	{
		return false;
	}
}

String JSONObject::valueToString (const Variant& value, int numtabs)
{
	String tabs;

	for (int i = 0; i < numtabs; ++i)
		tabs += '\t';

	switch (value.valueType())
	{
		case VARTYPE_Bool:
			return value.asBool() == true ? "true" : "false";

		case VARTYPE_Char:
			return String (value.asChar());

		case VARTYPE_UnsignedChar:
			return String ((char) value.asUnsignedChar());

		case VARTYPE_Double:
		case VARTYPE_Float:
		case VARTYPE_Int:
		case VARTYPE_Short:
		case VARTYPE_Long:
		case VARTYPE_UnsignedInt:
		case VARTYPE_UnsignedLong:
		case VARTYPE_UnsignedShort:
			return value.describe();

		case VARTYPE_List:
		{
			String listContents;

			for (const Variant& it : value.asList())
			{
				if (listContents.isEmpty() == false)
					listContents += ", ";

				listContents += valueToString (it, numtabs);
			}

			return "[" + listContents + "]";
		}

		case VARTYPE_Map:
		{
			String rep;
			const VariantMap& valAsMap = value.asMap();

			for (auto it = valAsMap.begin(); it != valAsMap.end(); ++it)
			{
				if (rep.isEmpty() == false)
					rep += ",\n";

				String name = it->first;
				String value = valueToString (it->second, numtabs + 1);
				rep += tabs + format ("\t\"%1\": %2", name, value);
			}

			return "{\n" + rep + "\n" + tabs + "}";
		}

		case VARTYPE_Pointer:
		{
			if (value.asPointer() == null)
				return "null";

			throw std::runtime_error ("cannot save non-null pointers to JSON");
		}

		case VARTYPE_String:
			return "\"" + value.asString() + "\"";

		case VARTYPE_None:
			return "null";
	}

	return "";
}
