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

#include "main.h"
#include "lexer.h"
#include "types.h"
#include "variant.h"
#include "json.h"

static String g_currentFile = "your face";

int main()
{
	JSONObject* obj = new JSONObject;
	VariantList values;
	values << 5;
	values << 1;
	values << 3.14;
	values << 9.12;

	obj->setProperty ("filename", __FILE__);
	obj->setProperty ("linenumber", __LINE__);
	obj->setProperty ("function", __PRETTY_FUNCTION__);
	obj->setProperty ("values", values);

	if (obj->saveToFile ("blargh.json") == false)
		printTo (stderr, "failed to save");

	/*

	JSONObject* obj = JSONObject::loadFromFile ("Housinator.json");

	if (obj == null)
	{
		printTo (stderr, "error: %1\n", JSONObject::loadError());
		return 1;
	}

	obj->saveToFile ("Housinator2.json");
	
	*/

	return 0;
}

const String& currentFileName()
{
	return g_currentFile;
}
