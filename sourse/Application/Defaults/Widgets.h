
#pragma once

#include "UI/GUI.h"

class Scroller : public Widget {

	Scroller& operator = (const Scroller& in);
	Scroller(const Scroller& in) : Widget(in) {
	}

public:
	
	virtual Scroller& Instance() {
		return *new Scroller(*this);
	}

	Scroller(Obj* prnt) : Widget(prnt, Rect<float>()) {
		RegisterType(ObjType("Scroller"));

		ADDOBJ(ColorObj, Inactive Col, *this, (this)).Set(Color(0.25f, 0.25f, 0.25f, 1.f));
		ADDOBJ(ColorObj, Active Col, *this, (this)).Set(Color(0.25f, 0.25f, 0.25f, 1.f));
		ADDOBJ(ColorObj, Scroll Col, *this, (this)).Set(Color(0.4f, 0.4f, 0.4f, 1.f));

		ADDOBJ(Bool, Vertical, *this, (this)).Set(true);

		Link* target = &ADDOBJ(Link, Target, *this, (this));
		target->Init("Widget", true);
		target->BindModPoll(this, TargetRemoveCallback);
		target->AddOnModCallBack(this, TargetAddCallback);
	}

	vec2<float> prev_crs;
	float scale_factor = 1;
	float content_offset = 0;

	void ProcBody(ObList* requests, TUI* tui, vec2<float> crs) {
		
		Widget* target = (Widget*)GETOBJ(Link, this, Target).GetLink();
		if (!target){
			return;
		}

		bool y_axis = GETOBJ(Bool, this, Vertical).GetVal();

		if (state == WidgetState::ACTIVATE) {
			prev_crs = crs;
			GETOBJ(Bool, this, Forse Active).Set(true);
		}
		else if (state == WidgetState::ACTIVE) {

			if (scale_factor < 1) {
				return;
			}

			vec2<float> delta = crs - prev_crs;
			float content_offset_delta = (&delta.x)[y_axis] * scale_factor;

			for (auto child : GETOBJ(ObList, target, Childs).GetList()) {
				Widget* widget = (Widget*)child.Data();
				if (widget->type.idname == "Scroller" || GETOBJ(Bool, widget, Hiden).GetVal()) {
					continue;
				}
				(&widget->rect.pos.x)[y_axis] += content_offset_delta;
				widget->skip_iteration = true;
			}

			prev_crs = crs;
		} 
		else {
			GETOBJ(Bool, this, Forse Active).Set(false);
		}
	}

	void DrawBody(Window& cnv, vec2<float> crs) {
		
		Widget* target = (Widget*)GETOBJ(Link, this, Target).GetLink();
		if (!target) {
			return;
		}

		if (scale_factor < 1) {
			return;
		}

		if (state == WidgetState::ACTIVE) {
			Color scroll_col;
			GETOBJ(ColorObj, this, Scroll Col).Get(&scroll_col);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), scroll_col, 7);
		}
		else if (state != WidgetState::NONE) {
			Color active_col;
			GETOBJ(ColorObj, this, Active Col).Get(&active_col);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), active_col, 7);
		}
		else {
			Color inactive_col;
			GETOBJ(ColorObj, this, Inactive Col).Get(&inactive_col);
			bool vertical = GETOBJ(Bool, this, Vertical).GetVal();
			(&rect.size.x)[!vertical] -= 5;
			cnv.RRect(Rect<float>(5 * vertical, 5 * !vertical, rect.size.x, rect.size.y), inactive_col, 3);
			(&rect.size.x)[!vertical] += 5;
		}
	}

	static bool TargetRemoveCallback(Obj* scroller) {
		Scroller* ths = (Scroller*)scroller;
		Widget* target = (Widget*)GETOBJ(Link, ths, Target).GetLink();
		if (!target) {
			return true;
		}

		target->RemoveOnModCallBack(Update);

		return true;
	}

	static void TargetAddCallback(Obj* scroller, ModType type) {
		Scroller* ths = (Scroller*)scroller;
		Widget* target = (Widget*)GETOBJ(Link, ths, Target).GetLink();
		if (!target) {
			return;
		}

		target->AddOnModCallBack(ths, Update);
	}

	static void Update(Obj* scroller, ModType type) {
		Scroller* ths = (Scroller*)scroller;

		Widget* target = (Widget*)GETOBJ(Link, ths, Target).GetLink();
		if (!target) {
			return;
		}

		bool y_axis = GETOBJ(Bool, ths, Vertical).GetVal();

		// updating rectangle
		ths->rect.pos.x = (target->rect.size.x - 20) * y_axis + 5;
		ths->rect.pos.y = (target->rect.size.y - 20) * (!y_axis) + 5;
		ths->rect.size.x = 20 * (y_axis)+target->rect.size.x * !y_axis - 10;
		ths->rect.size.y = 20 * (!y_axis) + target->rect.size.y * y_axis - 10;

		// finding content dimensions
		float min_val = FLT_MAX;
		float max_val = FLT_MIN;
		for (auto child : GETOBJ(ObList, target, Childs).GetList()) {
			Widget* widget = (Widget*)child.Data();

			if (widget->type.idname == "Scroller" || GETOBJ(Bool, widget, Hiden).GetVal()) {
				continue;
			}

			float max_pos = (&widget->rect.pos.x)[y_axis] + (&widget->rect.size.x)[y_axis];
			float min_pos = (&widget->rect.pos.x)[y_axis];

			if (max_pos > max_val) {
				max_val = max_pos;
			}
			if (min_pos < min_val) {
				min_val = min_pos;
			}
		}

		float content_size = max_val - min_val;
		float holder_size = target->rect.size.y;
		ABS(min_val);
		ths->content_offset = min_val;
		ths->scale_factor = content_size / (&target->rect.size.x)[y_axis];
	}

	virtual ~Scroller() {}
};


class InputField : public Widget {

	InputField& operator = (const InputField& in);
	InputField(const InputField& in);

public:

	virtual InputField& Instance() {
		return *new InputField(*this);
	}

	InputField(Obj* prnt, Rect<float> _rect);

	bool input = false;
	bool (*valid_input)(const Str* str) = nullptr;

	void ProcBody(ObList* requests, TUI* tui, vec2<float> crs);
	void DrawBody(Window& cnv, vec2<float> crs);

	virtual ~InputField() {}
};


class ListMenu : public Widget {

	ListMenu& operator = (const ListMenu& in);
	ListMenu(const ListMenu& in);

public:

	virtual ListMenu& Instance() {
		return *new ListMenu(*this);
	}

	ListMenu(Obj* prnt, Rect<float> _rect);

	float items_start = 40;
	float item_size = 25;

	void ProcBody(ObList* requests, TUI* tui, vec2<float> crs);
	void DrawBody(Window& cnv, vec2<float> crs);

	virtual ~ListMenu() {}
};

class ContextMenu : public Widget {

	ContextMenu& operator = (const ContextMenu& in);
	ContextMenu(const ContextMenu& in);

public:

	virtual ContextMenu& Instance() {
		return *new ContextMenu(*this);
	}

	ContextMenu(Obj* prnt, Rect<float> _rect);

	ListMenu* list_menu;
	InputField* input_field;
	float childs_start = 50;
	float child_height = 30;

	void ProcBody(ObList* requests, TUI* tui, vec2<float> crs);
	void DrawBody(Window& canvas, vec2<float> crs);

	static void TargetChanged(Obj* ths, ModType type);

	virtual ~ContextMenu() {}
};