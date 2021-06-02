#pragma once

#include "UI/UI.h"

#include "UI/Requester.h"
#include "TUI.h"
#include "Device/DevBuffer.h"

enum struct GuiiState {
	NONE = 0,
	ENTERED,
	INSIDE,
	LEAVED,
	ACTIVATE,
	CLOSE,
};

class Guii : public Requester {

	Guii& operator = (const Guii& in);

public:

	Guii(const Guii& in) : Requester(in) {
		childs = &GETOBJ(ObList, this, Childs).GetList();
	}

	Guii(Obj* prnt, Rect<float> _rect) : Requester(prnt) {
		RegisterType(ObjType("Guii"));

		ADDOBJ(ObList, Childs, *this, (this)).Assign("Guii", true);
		childs = &GETOBJ(ObList, this, Childs).GetList();


		Obj& rect_obj = ADDOBJ(Obj, Rect, *this, (this));
		rect_obj.BindModPoll(this, SetRectReq);
		rect_obj.AddOnModCallBack(this, RectMod);
		ADDOBJ(Float, Pos X, rect_obj, (&rect_obj)).Set(_rect.pos.x);
		ADDOBJ(Float, Pos Y, rect_obj, (&rect_obj)).Set(_rect.pos.y);
		ADDOBJ(Float, Size X, rect_obj, (&rect_obj)).Assign(_rect.size.x, 5, 2000);
		ADDOBJ(Float, Size Y, rect_obj, (&rect_obj)).Assign(_rect.size.y, 5, 2000);

		buff = new DevBuffer(_rect.size.x, _rect.size.y);
		rect = _rect;
	}

	virtual Guii& Instance() {
		return *new Guii(*this);
	}

	List<Obj>* childs;
	GuiiState state = GuiiState::NONE;
	DevBuffer* buff = nullptr;
	Rect<float> rect;

	virtual void ProcBody(ObList* requests) {}
	virtual void DrawBody(Obj* root_obj) {}
	virtual bool TransformRequest() { return false; }
	virtual void Transform() {}

	void Proc(ObList* requests, Obj* trigers, vec2<float> crs) {

		bool update = false;

		if (crs.x > 0 && crs.y > 0 && rect.size > crs) {

			bool activate = GETOBJ(CompareExpr, trigers, Activate).Evaluate();
			bool close = GETOBJ(CompareExpr, trigers, Close).Evaluate();

			if (activate) {
				state = GuiiState::ACTIVATE;
			}
			else if (close) {
				state = GuiiState::CLOSE;
			}
			else if (state == GuiiState::NONE) {
				state = GuiiState::ENTERED;
			}
			else {
				state = GuiiState::INSIDE;
			}

			update = true;

		}
		else {

			if (state == GuiiState::LEAVED || state == GuiiState::NONE) {
				state = GuiiState::NONE;
				return;
			}
			else {
				state = GuiiState::LEAVED;
				update = true;
			}
		}

		if (update) {

			ProcBody(requests);

			FOREACH_OBJ(childs, guii) {
				((Guii*)guii.Data())->Proc(requests, trigers, crs - rect.pos);
			}

		}
	}

	void Draw(Obj* root_obj, bool root) {

		DrawBody(root_obj);

		FOREACH_OBJ(childs, guii) {
			((Guii*)guii.Data())->Draw(root_obj, false);
		}


		if (!root) {
			Guii* prnt_guii = (Guii*)prnt;
			prnt_guii->buff->Project(buff, rect.pos);
		}
	}

	static bool SetRectReq(Obj* param) {
		return ((Guii*)param)->TransformRequest();
	}

	static void RectMod(Obj* param, ModType type) {
		((Guii*)param)->Transform();
	}

	virtual ~Guii() {

	}
};


class  GUI : public UI {

	GUI& operator = (const GUI& in);
	GUI(const GUI& in) : UI(in) {}

public:

	GUI(Obj* prnt, Device* _dev) : UI(prnt) {
		ADDOBJ(ObList, Windows, *this, (this)).Assign("Guii", true);

		Obj& Trigers = ADDOBJ(Obj, Trigers, *this, (this));

		ADDOBJ(CompareExpr, Activate, Trigers, (&Trigers));
		ADDOBJ(CompareExpr, Close, Trigers, (&Trigers));

		dev = _dev;
	}

	Device* dev;

	void PumpRequests(ObList* requests) {
		List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
		Obj* trigers = &GETOBJ(Obj, this, Trigers);

		vec2<float> crs;
		dev->GetCrsr(crs);

		FOREACH_OBJ(&windows, guii) {
			Guii* window = ((Guii*)guii.Data());
			window->Proc(requests, trigers, crs - window->rect.pos);
		}
	}

	void OutPut(Obj* root) {
		List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
		FOREACH_OBJ(&windows, guii) {
			((Guii*)guii.Data())->Draw(root, true);
		}

		dev->StartDraw();
		FOREACH_OBJ(&windows, guii) {
			Guii* window = ((Guii*)guii.Data());
			dev->DrawBuff(window->buff, window->rect.pos);
		}
	}

	~GUI() {}
};