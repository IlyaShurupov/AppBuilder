
#pragma once

#include "Object.h"

class String : public Obj {

	Str string;

	String& operator=(const String& in);

public:

	String(const String& in) : Obj(in) {
		string = in.string;
	}

	String(Obj* prnt) : Obj(prnt) {
		RegisterType(ObjType("String"));
	}

	virtual String& Instance() {
		return *new String(*this);
	}

	bool Assign(STRR _string) {
		if (!CanModify()) {
			return false;
		}
		string = _string;
		Modified(ModType::SET);
		return true;
	}

	bool Equal(const Obj& obj) {
		assert(type == obj.type);
		return string == ((String&)obj).GetStr();
	}

	Str& GetStr() {
		return string;
	}

	void as_string(Str* str) {
		*str = string;
	}

	bool from_string(Str* str) {
		return Assign(*str);
	}

	~String() {
	}
};
