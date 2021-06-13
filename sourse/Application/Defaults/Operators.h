#pragma once

#include "Thread/Thread.h"

class QuitProgram : public Operator {

	QuitProgram& operator = (const QuitProgram& in);
	QuitProgram(const QuitProgram& in) : Operator(in.prnt) {}

public:

	QuitProgram(Obj* prnt) : Operator(prnt) {
		RegisterType(ObjType("Quit Program"));
		ObDict& args = GETOBJ(ObDict, this, Interface);
		args.AddObj(new Bool(nullptr), "Save");
	}

	virtual QuitProgram& Instance() {
		return *new QuitProgram(*this);
	}

	bool Poll() { return true; }

	void Invoke() {
		DictObj* args = &((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();
		bool save = ((Bool*)args->Get("Save"))->GetVal();
		exit(0);
	}
};