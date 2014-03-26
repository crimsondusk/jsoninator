/*
	Copyright 2013-2014 Santeri Piippo
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

#include "variant.h"
#include "format.h"
#include "main.h"

#define DEFINE_VARIANT_TYPE(N, T) \
	Variant::Variant (T const& val) \
	{ \
		buildFrom##N (val); \
	} \
	\
	T Variant::as##N() const \
	{ \
		assert (valueType() == VARTYPE_##N); \
		return *reinterpret_cast<T*> (m_value); \
	} \
	\
	void Variant::buildFrom##N (const T& val)\
	{ \
		T* valptr = new T (val); \
		\
		/* Init the Variant */ \
		m_type = VARTYPE_##N; \
		m_typeName = #T; \
		m_allocsize = sizeof (T); \
		m_value = valptr; \
		\
		/* Write down the functions we'll clone or deinit this with */ \
		m_deInitFunction = Variant::deleteAs##N; \
		m_cloneFunction = Variant::cloneAs##N; \
	} \
	\
	void* Variant::cloneAs##N (const Variant& var)\
	{ \
		return new T (var.as##N()); \
	} \
	\
	void Variant::deleteAs##N (const void* val)\
	{ \
		const T* c_ptr = reinterpret_cast<const T*> (val); \
		delete const_cast<T*> (c_ptr); \
	}

DEFINE_VARIANT_TYPE (Int,			int)
DEFINE_VARIANT_TYPE (Short,			short)
DEFINE_VARIANT_TYPE (Long,			long)
DEFINE_VARIANT_TYPE (UnsignedInt,	uint)
DEFINE_VARIANT_TYPE (UnsignedShort,	ushort)
DEFINE_VARIANT_TYPE (UnsignedLong,	ulong)
DEFINE_VARIANT_TYPE (Char,			char)
DEFINE_VARIANT_TYPE (UnsignedChar,	uchar)
DEFINE_VARIANT_TYPE (Float,			float)
DEFINE_VARIANT_TYPE (Double,		double)
DEFINE_VARIANT_TYPE (Bool,			bool)
DEFINE_VARIANT_TYPE (String,		String)
DEFINE_VARIANT_TYPE (List,			VariantList)
DEFINE_VARIANT_TYPE (Map,			VariantMap)

// -----------------------------------------------------------------------------
//
Variant::Variant()
{
	buildFromString ("");
}

// -----------------------------------------------------------------------------
//
Variant::Variant (void* val)
{
	buildAsPointer (val, "void*");
}

// -----------------------------------------------------------------------------
//
Variant::Variant (const char* val)
{
	buildFromString (val);
}

// -----------------------------------------------------------------------------
//
Variant::Variant (std::nullptr_t)
{
	buildAsPointer (null, "std::nullptr_t");
}

// -----------------------------------------------------------------------------
//
Variant::Variant (const Variant& val)
{
	m_type = val.valueType();
	m_typeName = val.typeName();
	m_deInitFunction = val.deinitFunc();
	m_cloneFunction = val.cloneFunc();

	if (val.valueType() != VARTYPE_Pointer)
	{
		m_value = (*val.cloneFunc()) (val);
	}
	else
	{
		m_value = const_cast<void*> (val.asPointer());
		m_allocsize = 0;
	}
}

// -----------------------------------------------------------------------------
//
void Variant::buildAsPointer (void* val, const char* typestring)
{
	m_type = VARTYPE_Pointer;
	m_value = val;
	m_typeName = typestring;
	m_allocsize = 0;
}

// -----------------------------------------------------------------------------
//
int Variant::allocatedSize() const
{
	return m_allocsize;
}

// -----------------------------------------------------------------------------
//
const void* Variant::asPointer() const
{
	return m_value;
}

// -----------------------------------------------------------------------------
//
Variant::~Variant()
{
	(*m_deInitFunction) (asPointer());
}

// -----------------------------------------------------------------------------
//
const char* Variant::typeName() const
{
	return m_typeName;
}

// -----------------------------------------------------------------------------
//
VariantType Variant::valueType() const
{
	return m_type;
}

// -----------------------------------------------------------------------------
//
String Variant::describe() const
{
	switch (m_type)
	{
#define NUMERIC_STRINGREP(N, T, C) \
		case VARTYPE_##N: \
		{ \
			char valstr[64]; \
			sprintf (valstr, "%" #C, (T) as##N()); \
			return valstr; \
		}

		NUMERIC_STRINGREP (Char,			int,	d)
		NUMERIC_STRINGREP (Int,				int,	d)
		NUMERIC_STRINGREP (Short,			int,	d)
		NUMERIC_STRINGREP (Long,			long,	ld)
		NUMERIC_STRINGREP (UnsignedChar,	uint,	u)
		NUMERIC_STRINGREP (UnsignedInt,		uint,	u)
		NUMERIC_STRINGREP (UnsignedShort,	uint,	u)
		NUMERIC_STRINGREP (UnsignedLong,	ulong,	lu)
		NUMERIC_STRINGREP (Pointer,			void*,	p)

		case VARTYPE_Float:
			return String::fromNumber (asFloat());

		case VARTYPE_Double:
			return String::fromNumber (asDouble());

		case VARTYPE_List:
		{
			String data;

			for (const Variant& var : asList())
			{
				if (data.isEmpty() == false)
					data += ", ";

				data += var.describe();
			}

			return "{" + data + "}";
		}

		case VARTYPE_Map:
		{
			return FormatArgument (asMap()).text();
		}

		case VARTYPE_Bool:
			return asBool() ? "true" : "false";

		case VARTYPE_String:
			return asString();

		case VARTYPE_None:
			return "<invalid>";

		default:
			return String ("<") + typeName() + ">";
	}

	return "";
}
