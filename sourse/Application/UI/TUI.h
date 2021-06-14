
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

		ADDOBJ(Int, Key Code, *this, (this)).Assign(0, 0, 255);
		ADDOBJ(Int, State, *this, (this)).Assign(0, 0, 5);
		ADDOBJ(Bool, Is Text Input, *this, (this)).Assign(true);
		ADDOBJ(String, Char Val, *this, (this)).Assign("none");
		ADDOBJ(String, Shifted Char Val, *this, (this)).Assign("none");
	}

	virtual KeyInput& Instance() {
		return *new KeyInput(*this);
	}

	void Update(Keyboard* Device) {
		Int& state = GETOBJ(Int, this, State);
		int val = (int)Device->GetKeyState(GETOBJ(Int, this, Key Code).GetVal(), (InputState)state.GetVal());
		state.Set(val);
	}
};

class TUI : public UI {

	TUI& operator = (const TUI& in);
	TUI(const TUI& in) : UI(in) {
	}

	Str text_input = "";

public:

	TUI(Obj* prnt) : UI(prnt) {
		RegisterType(ObjType("TUI"));
		ADDOBJ(ObList, Shortcuts, *this, (this)).Assign("ShortCut", false);
		ADDOBJ(ObList, Inputs, *this, (this)).Assign("KeyInput", false);

		ADDOBJ(Link, Shift Key, *this, (this)).Init("KeyInput", true);
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


		text_input = "";

		KeyInput* shifted_key = (KeyInput*)GETOBJ(Link, this, Shift Key).GetLink();
		bool shifted = ((bool)shifted_key && (bool)GETOBJ(Int, shifted_key, State).GetVal());

		for (auto input_obj : GETOBJ(ObList, this, Inputs).GetList()) {
			KeyInput* input = (KeyInput*)input_obj.Data();

			if (GETOBJ(Bool, input, Is Text Input).GetVal()) {
				if (GETOBJ(Int, input, State).GetVal() == (int)InputState::PRESSED) {
					if (shifted) {
						text_input += GETOBJ(String, input, Shifted Char Val).GetStr();
					} 
					else {
						text_input += GETOBJ(String, input, Char Val).GetStr();
					} 
				}
			}
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

	const Str* get_text_input() {
		return &text_input;
	}

	~TUI() {}
};