
#pragma once

#include "Types.h"

#define STRR const Str&
#define FOREACH_OBJ(Oblist, iter) FOREACH(Oblist, Obj, iter)
#define ADDOBJ(Type, Name, to, Args) ((Type &)(to).AddChld(new Type Args, #Name))
#define GETOBJ(type, from, name) ((type&)*(from)->props.Get(#name))

class Obj;

struct CopyObj { Obj* operator()(Obj* in); };
using DictObj = Dict<Obj, 10, CopyObj>;

enum class ModType {
	SET,
	CILD,
	DELETE,
};

struct OnModCallBack {

	Obj* ths = nullptr;
	void (*callback)(Obj* ths, ModType type) = nullptr;

	OnModCallBack() {}

	OnModCallBack(Obj* _ths, void (*_callback)(Obj* ths, ModType type)) {
		callback = _callback;
		ths = _ths;
	}

	bool operator() (ModType type) {
		if (callback) {
			callback(ths, type);
			return true;
		}
		return false;
	}
};

struct ObjType {

	ObjType();
	ObjType(STRR name);

	bool IsPrnt(STRR);
	bool operator==(const ObjType& in) {
		return idname == in.idname;
	}

	Str idname;
	bool locked = false;
	ObjType* child = nullptr;
	ObjType* prnt = nullptr;
};

class Obj {

	Obj& operator=(const Obj& in);

public:

	Obj(const Obj& in);
	Obj(Obj* _prnt);

	virtual Obj& Instance() {
		return *new Obj(*this);
	}

	virtual ~Obj() {
		if (next) {
			next->next = prev;
		}
		if (prev) {
			prev->prev = next;
		}
	}

	Obj& GetChld(STRR idname);
	Obj& AddChld(Obj* chld, STRR idname);
	void DelChild(STRR idname);
	void RegisterType(const ObjType& _type);

	DictObj props;
	ObjType type;
	Obj* prnt = nullptr;
	Obj* next = nullptr;
	Obj* prev = nullptr;

	virtual bool Equal(const Obj& obj) { assert(type == obj.type); return false; }

	// modification callbacks

	Obj* req_mod_param = nullptr;
	bool (*req_mod_poll)(Obj* req_mod_param) = nullptr;
	Array<OnModCallBack> OnModCallBacks;

	void BindModPoll(Obj* ths, bool (*call)(Obj* ths)) {
		req_mod_poll = call;
		req_mod_param = ths;
	}

	bool CanModify() {
		if (type.locked) {
			return false;
		}
		if (req_mod_poll) {
			return req_mod_poll(req_mod_param);
		}
		return true;
	}


	void AddOnModCallBack(Obj* ths, void (*call)(Obj* ths, ModType)) {
		OnModCallBacks.PushBack(OnModCallBack(ths, call));
	}

	void Modified(ModType type) {
		for (int i = 0; i < OnModCallBacks.Len(); i++) {
			OnModCallBacks[i](type);
		}

		if (prnt) {
			prnt->Modified(ModType::CILD);
		}
	}

};

Obj* get_objs_entry();