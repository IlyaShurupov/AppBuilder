
#pragma once

#include "TypeTable.h"

TypeTable g_type_table;

ObjType::ObjType() {
	idname = "Obj";
}

ObjType::ObjType(STRR name) {
	idname = name;
}

bool ObjType::InheritsFrom(STRR prnt_id) {

	int inherit_type_idx = g_type_table.types.Presents(prnt_id);
	assert(inherit_type_idx != -1);
	ObjType* inherit_type = g_type_table.types.table[inherit_type_idx]->val;
	ObjType* type_node = this;

	while (type_node) {
		if (type_node == inherit_type) {
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
