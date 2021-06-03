#pragma once

#include "UI/UI.h"

#include "UI/Requester.h"
#include "TUI.h"
#include "Device/DevBuffer.h"

enum struct WidgetState {
	NONE = 0,
	ACTIVE,
	CONFIRM,
	DISCARD,
	ENTERED,
	INSIDE,
	LEAVED,
};

class Widget : public Requester {

	Widget& operator = (const Widget& in);

public:

	Widget(const Widget& in) : Requester(in) {
		*childs = GETOBJ(ObList, this, Childs).GetList();
	}

	Widget(Obj* prnt, Rect<float> _rect) : Requester(prnt) {
		RegisterType(ObjType("Widget"));

		ADDOBJ(ObList, Childs, *this, (this)).Assign("Widget", true);
		childs = &GETOBJ(ObList, this, Childs).GetList();


		Obj& rect_obj = ADDOBJ(Obj, Rect, *this, (this));
		rect_obj.BindModPoll(this, SetRectReq);
		rect_obj.AddOnModCallBack(this, RectMod);
		ADDOBJ(Float, Pos X, rect_obj, (&rect_obj)).Set(_rect.pos.x);
		ADDOBJ(Float, Pos Y, rect_obj, (&rect_obj)).Set(_rect.pos.y);
		ADDOBJ(Float, Size X, rect_obj, (&rect_obj)).Assign(_rect.size.x, 5, 2000);
		ADDOBJ(Float, Size Y, rect_obj, (&rect_obj)).Assign(_rect.size.y, 5, 2000);

		rect = _rect;

		buff = new DevBuffer(rect);
	}

	virtual Widget& Instance() {
		return *new Widget(*this);
	}

	List<Obj>* childs;
	WidgetState state = WidgetState::NONE;
	Rect<float> rect;
	bool redraw = false;
	DevBuffer* buff;

	virtual void ProcBody(ObList* requests) {}
	virtual void DrawBody(Obj* root_obj) {}
	virtual bool TransformRequest() { return false; }
	virtual void Transform() {}

	void Proc(ObList* requests, Obj* trigers, vec2<float> crs) {

		if (crs.x > 0 && crs.y > 0 && rect.size > crs) {

			bool confirm = GETOBJ(CompareExpr, trigers, Activate).Evaluate();
			bool discard = GETOBJ(CompareExpr, trigers, Close).Evaluate();

			if (confirm) {
				state = WidgetState::CONFIRM;
			}
			else if (discard) {
				state = WidgetState::DISCARD;
			}
			else if (state == WidgetState::NONE) {
				state = WidgetState::ENTERED;
			}
			else {
				state = WidgetState::INSIDE;
			}

			redraw = true;

		}
		else {

			if (state == WidgetState::LEAVED || state == WidgetState::NONE) {
				state = WidgetState::NONE;
				return;
			}
			else {
				state = WidgetState::LEAVED;
				redraw = true;
			}
		}

		if (redraw) {

			ProcBody(requests);

			FOREACH_OBJ(childs, guii) {
				((Widget*)guii.Data())->Proc(requests, trigers, crs - rect.pos);
			}

		}
	}

	void Draw(Obj* root_obj, bool root) {

		DrawBody(root_obj);

		FOREACH_OBJ(childs, guii) {
			((Widget*)guii.Data())->Draw(root_obj, false);
		}

		/*
		if (!root) {
			Guii* prnt_guii = (Guii*)prnt;
			prnt_guii->buff->Project(buff, rect.pos);
		}
		*/

		redraw = false;
	}

	static bool SetRectReq(Obj* param) {
		return ((Widget*)param)->TransformRequest();
	}

	static void RectMod(Obj* param, ModType type) {
		((Widget*)param)->Transform();
	}

	virtual ~Widget() {
	}
};


class  GUI : public UI {

	GUI& operator = (const GUI& in);
	GUI(const GUI& in) : UI(in) {}

public:

	GUI(Obj* prnt, Device* _dev) : UI(prnt) {
		ADDOBJ(ObList, Windows, *this, (this)).Assign("Widget", true);

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
			Widget* window = ((Widget*)guii.Data());
			window->Proc(requests, trigers, crs - window->rect.pos);
		}
	}

	void OutPut(Obj* root) {
		dev->StartDraw();


		List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
		FOREACH_OBJ(&windows, guii) {
			((Widget*)guii.Data())->Draw(root, true);
		}

		dev->EndDraw();

		/*
		FOREACH_OBJ(&windows, guii) {
			Guii* window = ((Guii*)guii.Data());
			dev->DrawBuff(window->buff, window->rect.pos);
		}
		*/
	}

	~GUI() {}
};