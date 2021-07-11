#pragma once

#include "Thread/Thread.h"

class QuitProgram : public Operator {

	QuitProgram& operator = (const QuitProgram& in);
	QuitProgram(const QuitProgram& in) : Operator(in.prnt) {}

public:

	QuitProgram(Obj* prnt) : Operator(prnt) {
		RegisterType(ObjType("Quit Program"));
		ObDict& args = GETOBJ(ObDict, this, Interface);
		args.AddObj(new Bool(&args), "Save");
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

class CopyObject : public Operator {

	CopyObject& operator = (const CopyObject& in);
	CopyObject(const CopyObject& in) : Operator(in) {
		int g = 0;
	}


public:

	CopyObject(Obj* prnt) : Operator(prnt) {
		RegisterType(ObjType("Copy Object"));
		
		ObDict& args = GETOBJ(ObDict, this, Interface);
		
		Link* dest = new Link(&args);
		args.AddObj(dest, "Destination");
		dest->Init("Obj", true);

		Link* target = new Link(&args);
		args.AddObj(target, "Target");
		dest->Init("Obj", true);
	}

	virtual CopyObject& Instance() {
		return *new CopyObject(*this);
	}

	bool Poll() { 
		DictObj& args = ((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();
		return ((Link*)args.Get("Destination"))->GetLink() && ((Link*)args.Get("Target"))->GetLink();
	}

	void Invoke() {
		DictObj& args = ((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();

		Obj* dest = ((Link*)args.Get("Destination"))->GetLink();
		Obj* target = ((Link*)args.Get("Target"))->GetLink();

		Obj* coppy = &target->Instance();
		dest->AddChld(coppy, "Clipboard Object");
		coppy->prnt = dest;


		state = OpState::FINISHED;
	}
};