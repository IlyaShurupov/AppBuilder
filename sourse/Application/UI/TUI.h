
#pragma once

#include "UI/UI.h"
#include "UI/Requester.h"

class CompareExpr : public Obj {
	CompareExpr& operator = (const CompareExpr& in);
	CompareExpr(const CompareExpr& in) : Obj(in) {}

public:

	CompareExpr(Obj* prnt) : Obj(prnt) {
		RegisterType(ObjType("Bool Compare Expretion"));

		ADDOBJ(ObList, Conditions, *this, (this)).Assign("Tuple", false);
		ADDOBJ(Bool, Or, *this, (this)).Assign(false);
	}

	bool Evaluate() {
		bool any = GETOBJ(Bool, this, Or).GetVal();

		List<Obj>& conditions = GETOBJ(ObList, this, Conditions).GetList();

		for (auto tpl_obj : conditions) {
			ObjTuple* tpl = (ObjTuple*)tpl_obj.Data();
			if (tpl->GetHead().GetLink()->Equal(*tpl->GetTail().GetLink())) {
				if (any) {
					return true;
				}
			}
			else if (!any) {
				return false;
			}
		}

		return true;
	}

	bool Equal(const Obj& obj) {
		assert(type == obj.type);
		return Evaluate() == ((CompareExpr&)obj).Evaluate();
	}
};

class ShortCut : public Requester {

	ShortCut& operator = (const ShortCut& in);
	ShortCut(const ShortCut& in) : Requester(in) {}

public:

	ShortCut(Obj* prnt) : Requester(prnt) {
		RegisterType(ObjType("ShortCut"));

		ADDOBJ(CompareExpr, Invoke, *this, (this));
		ADDOBJ(ObList, RunTimeArgs, *this, (this)).Assign("Tuple", false);
	}

	virtual ShortCut& Instance() {
		return *new ShortCut(*this);
	}

	void ProcInputs(ObList* reqs) {
		ObList& rtargs = GETOBJ(ObList, this, RunTimeArgs);
		CompareExpr& invoke = GETOBJ(CompareExpr, this, Invoke);

		if (Running()) {
			for(auto rtarg_obj : rtargs.GetList()) {
				ObjTuple* tpl = (ObjTuple*)rtarg_obj.Data();
				CompareExpr* cmpr = (CompareExpr*)tpl->GetTail().GetLink();

				if (cmpr->Evaluate()) {
					SendArg(*(String*)tpl->GetHead().GetLink());
				}
			}

		}
		else {
			CompareExpr& cmpr = GETOBJ(CompareExpr, this, Invoke);
			if (cmpr.Evaluate()) {
				CreateRequest(reqs);
			}
		}
	}
};

class KeyInput : public Obj {

	KeyInput& operator = (const KeyInput& in);
	KeyInput(const KeyInput& in) : Obj(in) {}

public:

	KeyInput(Obj* prnt) : Obj(prnt) {
		RegisterType(ObjType("KeyInput"));

		ADDOBJ(String, KeyName, *this, (this)).Assign("none");
		ADDOBJ(Int, ASCII Code, *this, (this)).Assign(0, 0, 255);
		ADDOBJ(Int, State, *this, (this)).Assign(0, 0, 5);
	}

	virtual KeyInput& Instance() {
		return *new KeyInput(*this);
	}

	void Update(Keyboard* Device) {
		Int& state = GETOBJ(Int, this, State);
		int val = (int)Device->GetKeyState(GETOBJ(Int, this, ASCII Code).GetVal(), (InputState)state.GetVal());
		state.Set(val);
	}
};

class TUI : public UI {

	TUI& operator = (const TUI& in);
	TUI(const TUI& in) : UI(in) {
	}

public:

	TUI(Obj* prnt) : UI(prnt) {
		ADDOBJ(ObList, Shortcuts, *this, (this)).Assign("ShortCut", false);
		ADDOBJ(ObList, Inputs, *this, (this)).Assign("KeyInput", false);
	}

	virtual TUI& Instance() {
		return *new TUI(*this);
	}

	Keyboard kb;

	void UpdateInputStates() {
		for (auto input_obj : GETOBJ(ObList, this, Inputs).GetList()) {
			KeyInput* input = (KeyInput*)input_obj.Data();
			input->Update(&kb);
		}
	}

	void PumpRequests(ObList* requests) {

		kb.PumpEvents();
		UpdateInputStates();
		kb.ClearEvents();

		for (auto shcut_obj : GETOBJ(ObList, this, Shortcuts).GetList()) {
			ShortCut* shcut = (ShortCut*)shcut_obj.Data();
			shcut->ProcInputs(requests);
		}
	}

	~TUI() {}
};