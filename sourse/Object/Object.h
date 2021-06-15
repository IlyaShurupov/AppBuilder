
#pragma once

#include "Types.h"

#define STRR const Str&
#define ADDOBJ(Type, Name, to, Args) ((Type &)(to).AddChld(new Type Args, #Name))
#define GETOBJ(type, from, name) ((type&)*(from)->props.Get(#name))

class Obj;

struct CopyObj { 
	Obj* operator()(Obj* in);
};

using DictObj = Dict<Obj*, CopyObj>;

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

	void BindModPoll(Obj* ths, bool (*call)(Obj* ths));
	bool CanModify();

	void AddOnModCallBack(Obj* ths, void (*call)(Obj* ths, ModType));
	void Modified(ModType type);

	virtual void as_string(Str* str) { *str = "Object Base Class"; }
	virtual bool from_string(Str* str) { return false; }

	virtual ~Obj() {
		if (next) {
			next->next = prev;
		}
		if (prev) {
			prev->prev = next;
		}
	}
};

Obj* get_objs_entry();