#pragma once

#include "UI/UI.h"

#include "UI/Requester.h"
#include "TUI.h"
#include "Window/Window.h"

enum struct WidgetState {
	NONE = 0,
	ENTERED,
	INSIDE,
	FORSED_ACTIVE,
	ACTIVATE,
	ACTIVE,
	CONFIRM,
	DISCARD,
	LEAVED,
};

struct WidgetTriggers {

	bool activate = false;
	bool comfirm = false;
	bool discard = false;

	bool handled = false;

	void Update(Obj* trigers) {
		activate = GETOBJ(CompareExpr, trigers, Activate).Evaluate();
		comfirm = GETOBJ(CompareExpr, trigers, Comfirm).Evaluate();
		discard = GETOBJ(CompareExpr, trigers, Discard).Evaluate();
		handled = false;
	}

};


class Widget : public Requester {

	Widget& operator = (const Widget& in);

public:

	Widget(const Widget& in);

	Widget(Obj* prnt, Rect<float> _rect, const Str& descrip = " ");

	virtual Widget& Instance() {
		return *new Widget(*this);
	}

	List<Obj>* childs;
	Rect<float> rect;

	vec2<float> local_crs;
	WidgetState state = WidgetState::NONE;
	bool active = true;

	virtual bool IsActive(vec2<float> crs);
	void Proc(ObList* requests, WidgetTriggers* triggers, TUI* tui);
	void Draw(Window& canvas, vec2<float> prnt_pos, const Rect<float>& draw_bounds);

	virtual void ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers) {}
	virtual void DrawBody(Window& canvas, vec2<float> crs) {}
	
	virtual bool TransformRequest() { return true; }

	virtual void Transform() {
		Obj* rect_obj = &GETOBJ(Obj, this, Rect);
		rect.pos.x = GETOBJ(Float, rect_obj, Pos X).GetVal();
		rect.pos.y = GETOBJ(Float, rect_obj, Pos Y).GetVal();
		rect.size.x = GETOBJ(Float, rect_obj, Size X).GetVal();
		rect.size.y = GETOBJ(Float, rect_obj, Size Y).GetVal();
	}

	void ApplyRect() {
		Obj* rect_obj = &GETOBJ(Obj, this, Rect);
		GETOBJ(Float, rect_obj, Pos X).Set(rect.pos.x);
		GETOBJ(Float, rect_obj, Pos Y).Set(rect.pos.y);
		GETOBJ(Float, rect_obj, Size X).Set(rect.size.x);
		GETOBJ(Float, rect_obj, Size Y).Set(rect.size.y);
	}

	void UpdateRect() {
		Obj* rect_obj = &GETOBJ(Obj, this, Rect);
		rect.pos.x = GETOBJ(Float, rect_obj, Pos X).GetVal();
		rect.pos.y = GETOBJ(Float, rect_obj, Pos Y).GetVal();
		rect.size.x = GETOBJ(Float, rect_obj, Size X).GetVal();
		rect.size.y = GETOBJ(Float, rect_obj, Size Y).GetVal();
	}

	static bool SetRectReq(Obj* param);
	static void RectMod(Obj* param, ModType type);

	virtual ~Widget() {
	}

	virtual Str as_string() {
		return (type.idname + " ") += GETOBJ(String, this, Description).GetStr();
	}
};


class  GUI : public UI {

	GUI& operator = (const GUI& in);
	GUI(const GUI& in) : UI(in) {}

	WidgetTriggers triggers;
	Window canvas;
	TUI* tui;

public:

	GUI(Obj* prnt, TUI* tui);

	void PumpRequests(ObList* requests);
	void OutPut(Obj* root);

	virtual ~GUI() {}
};