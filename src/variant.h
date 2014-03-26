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

#pragma once
#include <string.h>
#include "string.h"
#include "types.h"

#define DECLARE_VARIANT_TYPE(N, T) \
public: \
	Variant (const T& val); \
	T as##N() const; \
private: \
	void buildFrom##N (const T& val); \
	static void* cloneAs##N (Variant const& var); \
	static void deleteAs##N (const void* val);

class Variant;
using VariantList = List<Variant>;
using VariantMap = Map<String, Variant>;

enum VariantType
{
	VARTYPE_None,
	VARTYPE_Int,
	VARTYPE_Short,
	VARTYPE_Long,
	VARTYPE_UnsignedInt,
	VARTYPE_UnsignedShort,
	VARTYPE_UnsignedLong,
	VARTYPE_Char,
	VARTYPE_UnsignedChar,
	VARTYPE_Float,
	VARTYPE_Double,
	VARTYPE_Bool,
	VARTYPE_String,
	VARTYPE_List,
	VARTYPE_Map,
	VARTYPE_Pointer,
};

class Variant
{
public:
	using DeInitFunction = void (*) (const void*);
	using CloneFunction = void* (*) (const Variant&);

	DECLARE_VARIANT_TYPE (Int,				int)
	DECLARE_VARIANT_TYPE (Short,			short)
	DECLARE_VARIANT_TYPE (Long,				long)
	DECLARE_VARIANT_TYPE (UnsignedInt,		uint)
	DECLARE_VARIANT_TYPE (UnsignedShort,	ushort)
	DECLARE_VARIANT_TYPE (UnsignedLong,		ulong)
	DECLARE_VARIANT_TYPE (Char,				char)
	DECLARE_VARIANT_TYPE (UnsignedChar,		uchar)
	DECLARE_VARIANT_TYPE (Float,			float)
	DECLARE_VARIANT_TYPE (Double,			double)
	DECLARE_VARIANT_TYPE (Bool,				bool)
	DECLARE_VARIANT_TYPE (String,			String)
	DECLARE_VARIANT_TYPE (List,				VariantList)
	DECLARE_VARIANT_TYPE (Map,				VariantMap)

public:
	Variant();
	Variant (void* val);
	Variant (const char* val);
	Variant (std::nullptr_t);
	Variant (const Variant& val);
	~Variant();

	void			buildAsPointer (void* val, const char* typestring);
	int				allocatedSize() const;
	const void*		asPointer() const;
	String			describe() const;
	const char*		typeName() const;
	VariantType		valueType() const;

	inline CloneFunction cloneFunc() const
	{
		return m_cloneFunction;
	}

	inline DeInitFunction deinitFunc() const
	{
		return m_deInitFunction;
	}

private:
	int				m_allocsize;
	void*			m_value;
	const char*		m_typeName;
	VariantType		m_type;
	DeInitFunction	m_deInitFunction;
	CloneFunction	m_cloneFunction;
};
