
#pragma once

#include "Object.h"

class String : public Obj {

	Str string;

	String& operator=(const String& in);

public:

	String(const String& in);
	String(Obj* prnt);

	virtual String& Instance() {
		return *new String(*this);
	}

	bool Assign(STRR _string);
	bool Equal(const Obj& obj);
	Str& GetStr();
	void as_string(Str* str);
	bool from_string(Str* str);
	~String();
};


class ObList : public Obj {

	List<Obj> list;
	Str list_type;
	bool base_class = false;

	ObList& operator = (const ObList& in);

public:

	ObList(const ObList& in);
	ObList(Obj* prnt);
	virtual ObList& Instance() {
		return *new ObList(*this);
	}

	ObList& Assign(Str _list_type, bool _base_class);
	List<Obj>& GetList();
	bool AddObj(Obj* obj);
	const Str& ListType();
	~ObList();
};

class Link : public Obj {

	Obj* link = nullptr;
	Str link_type = "Obj";
	Str current_type = "None";
	bool base_class = true;

	Link& operator = (const Link& in);

public:

	Link(const Link& in);
	Link(Obj* prnt);
	virtual Link& Instance() {
		return *new Link(*this);
	}

	void Init(STRR _link_type, bool _base_class);
	bool SetLink(Obj* obj);
	bool Equal(const Obj& obj);
	Obj* GetLink();
	~Link();
};


class Int : public Obj {

	aligned val = 0;

	Int& operator = (const Int& in);

public:

	Int(const Int& in);

	aligned max = ALIGNED_MAX;
	aligned min = ALIGNED_MIN;

	Int(Obj* prnt);
	virtual Int& Instance() {
		return *new Int(*this);
	}

	bool Assign(aligned _val, aligned _min, aligned _max);
	aligned GetVal();
	bool Set(aligned _val);
	bool Equal(const Obj& obj);
	void as_string(Str* str);
	bool from_string(Str* str);

	~Int();
};

class Float : public Obj {

	float val = 0;

	Float& operator = (const Float& in);

public:

	Float(const Float& in);

	float max = FLT_MAX;
	float min = FLT_MIN;

	Float(Obj* prnt);
	virtual Float& Instance() {
		return *new Float(*this);
	}

	bool Assign(float _val, float _min, float _max);
	float GetVal();
	bool Set(float _val);
	bool Equal(const Obj& obj);
	void as_string(Str* str);
	bool from_string(Str* str);
	~Float();
};

class Bool : public Obj {

	bool val = 0;

	Bool& operator = (const Bool& in);

public:

	Bool(const Bool& in);
	Bool(Obj* prnt);

	virtual Bool& Instance() {
		return *new Bool(*this);
	}

	Bool& Assign(bool _val);
	bool GetVal();
	bool Set(bool _val);
	bool Equal(const Obj& obj);
	void as_string(Str* str); 
	bool from_string(Str* str);
	~Bool();
};


class ObDict : public Obj {

	DictObj dict;
	Str dict_type;
	bool base_class = false;

	ObDict& operator = (const ObDict& in);

public:

	ObDict(const ObDict& in);
	ObDict(Obj* prnt);
	virtual ObDict& Instance() {
		return *new ObDict(*this);
	}

	ObDict& Assign(Str _dict_type, bool _base_class);
	DictObj& GetDict();
	bool AddObj(Obj* obj, STRR name);
	Obj& GetObj(STRR name);
	~ObDict();
};

class ObjTuple : public Obj {

	ObjTuple& operator=(const ObjTuple& in);

public:

	ObjTuple(const ObjTuple& in);

	ObjTuple(Obj* prnt);

	virtual ObjTuple& Instance() {
		return *new ObjTuple(*this);
	}

	Link& GetHead();
	Link& GetTail();
	~ObjTuple();
};


class CompareExpr : public Obj {
	CompareExpr& operator = (const CompareExpr& in);
	CompareExpr(const CompareExpr& in);

public:

	CompareExpr(Obj* prnt);

	bool Evaluate();
	bool Equal(const Obj& obj);
};