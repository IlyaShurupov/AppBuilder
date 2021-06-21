#pragma once

#include "UI/UI.h"

#include "UI/Requester.h"
#include "TUI.h"
#include "Window/Window.h"

enum struct WidgetState {
	NONE = 0,
	ACTIVATE,
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

	Widget(const Widget& in);

	Widget(Obj* prnt, Rect<float> _rect);

	virtual Widget& Instance() {
		return *new Widget(*this);
	}

	List<Obj>* childs;
	WidgetState state = WidgetState::NONE;
	Rect<float> rect;
	bool active = false;
	bool skip_iteration = false;

	virtual void ProcBody(ObList* requests, TUI* tui, vec2<float> crs) {}
	virtual void DrawBody(Window& canvas, vec2<float> crs) {}
	virtual bool TransformRequest() { return true; }

	virtual void Transform() {
		Obj* rect_obj = &GETOBJ(Obj, this, Rect);
		rect.pos.x = GETOBJ(Float, rect_obj, Pos X).GetVal();
		rect.pos.y = GETOBJ(Float, rect_obj, Pos Y).GetVal();
		rect.size.x = GETOBJ(Float, rect_obj, Size X).GetVal();
		rect.size.y = GETOBJ(Float, rect_obj, Size Y).GetVal();
	}

	void Proc(ObList* requests, Obj* trigers, TUI* tui, vec2<float> crs);

	void Draw(Window& canvas, vec2<float> prnt_pos, vec2<float> crs);

	static bool SetRectReq(Obj* param);

	static void RectMod(Obj* param, ModType type);

	virtual ~Widget() {
	}
};


class  GUI : public UI {

	GUI& operator = (const GUI& in);
	GUI(const GUI& in) : UI(in) {}

	Window canvas;
	TUI* tui;

public:

	GUI(Obj* prnt, TUI* tui);

	void PumpRequests(ObList* requests);
	void OutPut(Obj* root);

	virtual ~GUI() {}
};