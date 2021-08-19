
#pragma once

#include "Types.h"

struct ObjType {

	ObjType();
	ObjType(STRR name);

	bool InheritsFrom(STRR);
	bool operator==(const ObjType& in) {
		return idname == in.idname;
	}

	Str idname;
	ObjType* prnt = nullptr;
};

struct TypeTable {
	Dict<ObjType*> types;

	ObjType* define_type(const Str& type_name);
};