#pragma once

#include "UI/UI.h"

#include "UI/Requester.h"
#include "TUI.h"

#include "nanovg.h"

class WidgetCanvas {

	Rect<float> wrld_rec;
	NVGcontext* vg = nullptr;

public:

	WidgetCanvas(class NVGcontext* vg);

	void SetBounds(const Rect<float>& wrld_rec);

	void DrawRect(const Rect<float>& rect, const Color& col, float radius = 0);
	void DrawBounds(const Rect<float>& rect, const Color& col, short thickness);
	void DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness);
	void DrawText(const char* str, const float x, float y, float font_scale, const Color& col);
	void Clear(const Color& col);

	~WidgetCanvas();
};

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
	virtual void DrawBody(WidgetCanvas& canvas) {}
	virtual bool TransformRequest() { return false; }
	virtual void Transform() {}

	void Proc(ObList* requests, Obj* trigers, vec2<float> crs);

	void Draw(WidgetCanvas& canvas, vec2<float> prnt_pos);

	static bool SetRectReq(Obj* param);

	static void RectMod(Obj* param, ModType type);

	virtual ~Widget() {
	}
};


class  GUI : public UI {

	GUI& operator = (const GUI& in);
	GUI(const GUI& in) : UI(in) {}

	WidgetCanvas* canvas = nullptr;

public:

	GUI(Obj* prnt, Device* _dev);

	NVGcontext* vg;
	Device* dev;

	void PumpRequests(ObList* requests);
	void OutPut(Obj* root);

	virtual ~GUI() {}
};