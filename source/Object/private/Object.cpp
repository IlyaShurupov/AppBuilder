
#include "Object.h"

Obj* objs_entry = nullptr;

extern TypeTable g_type_table;

Obj* CopyObj::operator()(Obj* in) {
	return &in->Instance();
}

void Obj::set_obj_entry(Obj* new_entry) {
	objs_entry = new_entry;
}

Obj::Obj(const Obj& in) {
	props = in.props;

	type = in.type;
	prnt = in.prnt;

	req_mod_param = in.req_mod_param;
	req_mod_poll = in.req_mod_poll;
	OnModCallBacks = in.OnModCallBacks;

	if (objs_entry) {
		objs_entry->next = this;
		prev = objs_entry;
	}

	objs_entry = this;

	for (auto child : props) {
		child->val->prnt = this;
	}
}

Obj::Obj(Obj* _prnt) {
	type = g_type_table.define_type("Obj");
	prnt = _prnt;

	if (objs_entry) {
		objs_entry->next = this;
		prev = objs_entry;
	}

	objs_entry = this;
}

ObjType* Obj::RegisterType(const Str& _type) {
	ObjType* current_type = type;
	type = g_type_table.define_type(_type);
	type->prnt = current_type;
	return type;
}

Obj& Obj::GetChld(STRR idname) {
	return *props.Get(idname);
}

Obj& Obj::AddChld(Obj* val, STRR idname) {
	props.Put(idname, val);
	return *val;
}

void Obj::DelChild(STRR idname) {
	props.Remove(idname);
}

Obj* get_objs_entry() {
	return objs_entry;
}

void Obj::BindModPoll(Obj* ths, bool (*call)(Obj* ths)) {
	req_mod_poll = call;
	req_mod_param = ths;
}

bool Obj::CanModify() {
	if (req_mod_poll) {
		return req_mod_poll(req_mod_param);
	}
	return true;
}


void Obj::AddOnModCallBack(Obj* ths, void (*call)(Obj* ths, ModType)) {
	OnModCallBacks.PushBack(OnModCallBack(ths, call));
}

void Obj::RemoveOnModCallBack(void (*call)(Obj* ths, ModType)) {
	for (uint4 i = 0; i < OnModCallBacks.Len(); i ++) {
		if (OnModCallBacks[i].callback == call) {
			OnModCallBacks.Remove(i);
		}
	}
}

void Obj::Modified(ModType type) {
	for (uint4 i = 0; i < OnModCallBacks.Len(); i++) {
		OnModCallBacks[i](type);
	}
}