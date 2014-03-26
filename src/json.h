#pragma once
#include "main.h"
#include "variant.h"

class JSONObject
{
public:
	JSONObject();
	~JSONObject();

	bool				isEmpty() const;
	bool				hasProperty (const String& name);
	const Variant&		property (const String& name);
	void				setProperty (const String& name, const Variant& value);
	const VariantMap&	values() const;
	bool				saveToFile (const String& fileName);

	static JSONObject*	loadFromFile (const String& name);
	static String		loadError();

private:
	VariantMap	m_values;

	String				toString (int tabs = 0);
	String				valueToString (const Variant& value, int numtabs);
};
