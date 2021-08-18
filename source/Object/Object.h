
#pragma once

#include "Types.h"

#include "TypeTable.h"

#define ADDOBJ(Type, Name, to, Args) ((Type &)(to).AddChld(new Type Args, #Name))
#define GETOBJ(type, from, name) ((type&)*(from)->props.Get(#name))

class Obj;

struct CopyObj { 
	Obj* operator()(Obj* in);
};

using DictObj = Dict<Obj*, CopyObj>;

enum class ModType {
	SET,
	CHILD,
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

class Obj {

	Obj& operator=(const Obj& in);

	void set_obj_entry(Obj* new_entry);

public:

	Obj(const Obj& in);
	Obj(Obj* _prnt);

	virtual Obj& Instance() {
		return *new Obj(*this);
	}

	Obj& GetChld(STRR idname);
	Obj& AddChld(Obj* chld, STRR idname);
	void DelChild(STRR idname);
	ObjType* RegisterType(const Str& _type);

	DictObj props;
	ObjType* type = nullptr;
	Obj* prnt = nullptr;
	Obj* next = nullptr;
	Obj* prev = nullptr;

	virtual bool Equal(const Obj& obj) { return false; }

	// modification callbacks

	Obj* req_mod_param = nullptr;
	bool (*req_mod_poll)(Obj* req_mod_param) = nullptr;
	Array<OnModCallBack> OnModCallBacks;

	void BindModPoll(Obj* ths, bool (*call)(Obj* ths));
	bool CanModify();

	void AddOnModCallBack(Obj* ths, void (*call)(Obj* ths, ModType));
	void RemoveOnModCallBack(void (*call)(Obj* ths, ModType));
	void Modified(ModType type);

	virtual Str as_string() { 
		return type->idname; 
	}

	virtual bool from_string(Str* str) { return false; }

	// returns size written
	virtual int save_to_file(int (*save_obj)(File* file, Obj* obj), File* file, int file_adress) {
		uint8 size = save_static_self(file, file_adress);
		size += save_dyn_alloc_to_file(file, file_adress);
		return size;
	}

	// returns size written
	virtual int save_static_self(File* file, int save_adress) {
		int self_static_size = sizeof(Obj);
		file->write((uint1 *)this, self_static_size, save_adress);
		return self_static_size;
	}

	// returns size written
	virtual int save_dyn_alloc_to_file(File* file, int self_adress) {
		return 0;
	}

	// no size needed
	virtual void save_oblinks(int (*save_obj)(File* file, Obj* obj), File* file, int self_adress) {
		int test = S_OFFSET(Obj, prev) - S_OFFSET(Obj, next);
		test++;
	}


	virtual ~Obj() {
		if (next) {
			next->prev = prev;
		}
		else {
			set_obj_entry(prev);
		}
		
		if (prev) {
			prev->next = next;
		}
	}
};

Obj* get_objs_entry();