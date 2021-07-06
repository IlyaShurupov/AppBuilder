
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

class ObListIter {
public:

	Str itertype;
	bool type = false;
	int idx = 0;
	class ObList* oblist;
	Node<Obj>* iter;

	ObListIter(const Str* itertype, class ObList* oblist, int start_idx);

	void operator ++();

	Obj* data() {
		return iter->data;
	}

	Str* name() {
		return nullptr;
	}

	int Len(const Str& type);
	int Len();

	~ObListIter() {
	}
};

class ObList : public Obj {

	List<Obj> list;
	Str list_type;
	bool base_class = false;

	ObList& operator = (const ObList& in);

public:

	friend ObListIter;

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

	ObListIter Iterator(const Str& itertype, int start = 0) {
		return ObListIter(&itertype, this, start);
	}

	ObListIter Iterator(int start = 0) {
		return ObListIter(nullptr, this, start);
	}
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
	float min = -FLT_MAX;

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


class ObDictIter {
public:

	Str itertype;
	bool type = false;
	int slot_idx = 0;
	int idx = 0;
	class ObDict* obdict;

	ObDictIter(const Str* itertype, class ObDict* oblist, int start_idx);

	void operator ++();

	int Len(const Str& type);
	int Len();

	Obj* data();

	Str* name();

	~ObDictIter() {
	}
};

class ObDict : public Obj {

	DictObj* dict;
	Str dict_type;
	bool base_class = false;

	ObDict& operator = (const ObDict& in);

public:

	friend ObDictIter;

	ObDict(const ObDict& in);
	ObDict(Obj* prnt);
	virtual ObDict& Instance() {
		return *new ObDict(*this);
	}

	ObDict& Assign(Str _dict_type, bool _base_class);
	DictObj& GetDict();
	bool AddObj(Obj* obj, STRR name);
	Obj& GetObj(STRR name);

	ObDictIter Iterator(const Str& itertype, int start = 0) {
		return ObDictIter(&itertype, this, start);
	}

	ObDictIter Iterator(int start = 0) {
		return ObDictIter(nullptr, this, start);
	}

	bool SetRef(DictObj* new_dict, bool del_prev = true) {
		if (del_prev) {
			delete dict;
		}
		dict = new_dict;
		return true;
	}

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


class ColorObj : public Obj {

	bool val = 0;

	ColorObj& operator = (const ColorObj& in);

public:

	ColorObj(Obj* prnt);

	virtual ColorObj& Instance() {
		return *new ColorObj(*this);
	}

	void Get(Color* col);
	bool Set(const Color& col);
	bool Equal(const Obj& obj);
	
	~ColorObj() {}
};