#pragma once

#include "UI/UI.h"

#include "UI/Requester.h"
#include "TUI.h"
#include "Window/Window.h"

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

	Widget(const Widget& in);

	Widget(Obj* prnt, Rect<float> _rect);

	virtual Widget& Instance() {
		return *new Widget(*this);
	}

	List<Obj>* childs;
	WidgetState state = WidgetState::NONE;
	Rect<float> rect;
	bool redraw = false;

	virtual void ProcBody(ObList* requests) {}
	virtual void DrawBody(Window& canvas) {}
	virtual bool TransformRequest() { return false; }
	virtual void Transform() {}

	void Proc(ObList* requests, Obj* trigers, vec2<float> crs);

	void Draw(Window& canvas, vec2<float> prnt_pos);

	static bool SetRectReq(Obj* param);

	static void RectMod(Obj* param, ModType type);

	virtual ~Widget() {
	}
};


class  GUI : public UI {

	GUI& operator = (const GUI& in);
	GUI(const GUI& in) : UI(in) {}

	Window canvas;

public:

	GUI(Obj* prnt);

	void PumpRequests(ObList* requests);
	void OutPut(Obj* root);

	virtual ~GUI() {}
};