
#pragma once

#include "TypeTable.h"

TypeTable g_type_table;

ObjType::ObjType() {
	idname = "Obj";
}

ObjType::ObjType(STRR name) {
	idname = name;
}

bool ObjType::IsPrnt(STRR prnt_id) {
	ObjType* type_node = this;
	while (type_node) {
		if (type_node->idname == prnt_id) {
			return true;
		}
		type_node = type_node->prnt;
	}
	return false;
}

// BAD! does not check for redefenition
ObjType* TypeTable::define_type(const Str& type_name) {
	ObjType* out = nullptr;
	int slot_idx = types.Presents(type_name);

	if (slot_idx == -1) {
		out = new ObjType(type_name);
		types.Put(type_name, out);
	}
	else {
		out = types.table[slot_idx]->val;
	}

	return out;
}
