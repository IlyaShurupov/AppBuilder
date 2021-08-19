
#include "UI/GUI.h"

Widget::Widget(const Widget& in) : Requester(in) {
	childs = &GETOBJ(ObList, this, Childs).GetList();
}

Widget::Widget(Obj* prnt, Rect<float> _rect, const Str& descrip) : Requester(prnt) {
	RegisterType("Widget");

	ADDOBJ(ObList, Childs, *this, (this)).Assign("Widget", true);
	childs = &GETOBJ(ObList, this, Childs).GetList();

	Obj& rect_obj = ADDOBJ(Obj, Rect, *this, (this));
	ADDOBJ(Float, Pos X, rect_obj, (&rect_obj)).Set(_rect.pos.x);
	ADDOBJ(Float, Pos Y, rect_obj, (&rect_obj)).Set(_rect.pos.y);
	ADDOBJ(Float, Size X, rect_obj, (&rect_obj)).Assign(_rect.size.x, 5, 2000);
	ADDOBJ(Float, Size Y, rect_obj, (&rect_obj)).Assign(_rect.size.y, 5, 2000);

	rect = _rect;

	ADDOBJ(Bool, Hiden, *this, (this)).Set(false);
	ADDOBJ(Bool, Forse Active, *this, (this)).Set(false);
	ADDOBJ(Bool, Need Update, *this, (this)).Set(false);

	ADDOBJ(Int, DrawOrder, *this, (this));
	ADDOBJ(Int, ProcOrder, *this, (this));


	ADDOBJ(String, Description, *this, (this)).Assign(descrip);
}

bool Widget::IsActive(vec2<float> crs) {
	UpdateRect();

	if (GETOBJ(Bool, this, Hiden).GetVal()) {
		return false;
	}

	bool active = false;

	if (crs.x > 0 && crs.y > 0 && rect.size > crs) {
		active = true;
	}
	else if (state != WidgetState::NONE) {
		active = true;
	}

	return active;
}

void Widget::Proc(ObList* requests, WidgetTriggers* triggers, TUI* tui) {
	
	UpdateRect();

	bool inside = local_crs.x > 0 && local_crs.y > 0 && rect.size > local_crs;
	
	switch (state) {
		case WidgetState::NONE:
			if (inside) {
				state = WidgetState::ENTERED;
			}
			else if (GETOBJ(Bool, this, Forse Active).GetVal()) {
				state = WidgetState::FORSED_ACTIVE;
			}
			break;

		case WidgetState::ENTERED:
			state = WidgetState::INSIDE;
			break;

		case WidgetState::INSIDE:
			if (inside) {
				if (!triggers->handled && triggers->activate) {
					state = WidgetState::ACTIVATE;
				}
			}
			else {
				state = WidgetState::LEAVED;
			}
			break;

		case WidgetState::LEAVED:
			state = WidgetState::NONE;
			break;

		case WidgetState::ACTIVATE:
			state = WidgetState::ACTIVE;
			break;

		case WidgetState::ACTIVE:
			if (!triggers->handled && triggers->comfirm) {
				state = WidgetState::CONFIRM;
			}
			else if (!triggers->handled && triggers->discard) {
				state = WidgetState::DISCARD;
			}
			break;

		case WidgetState::DISCARD:
		case WidgetState::CONFIRM:
			if (inside) {
				state = WidgetState::INSIDE;
			}
			else {
				state = WidgetState::LEAVED;
			}
			break;

		case WidgetState::FORSED_ACTIVE:
			if (!GETOBJ(Bool, this, Forse Active).GetVal()) {
				state = WidgetState::NONE;
			}
			else {
				if (!triggers->handled && triggers->activate) {
					state = WidgetState::ACTIVATE;
				}
			}
			break;
	}


	ProcBody(requests, tui, triggers);

	ApplyRect();
}

void Widget::Draw(Window& canvas, vec2<float> prnt_pos, const Rect<float>& draw_bounds) {

	rect.pos += prnt_pos;
	canvas.SetCanvasRect(rect);
	Rect<float> clamped_bounds(draw_bounds);
	if (prnt && prnt->type->InheritsFrom("Widget")) {
		Rect<float> prnt_rect(prnt_pos, ((Widget*)prnt)->rect.size);
		if (!prnt_rect.overlap(rect)) {
			rect.pos -= prnt_pos;
			return;
		}
		clamped_bounds.clamp(prnt_rect);
	}
	canvas.SetBounds(clamped_bounds);
	rect.pos -= prnt_pos;


	DrawBody(canvas, local_crs);

	childs->Sort([](const Obj& ob1, const Obj& ob2) {
		return GETOBJ(Int, ((Widget*)&ob1), DrawOrder).GetVal() > GETOBJ(Int, ((Widget*)&ob2), DrawOrder).GetVal();
	});

	for (auto guii : *childs) {
		Widget* child = (Widget*)guii.Data();
		if (!GETOBJ(Bool, child, Hiden).GetVal()) {
			child->Draw(canvas, prnt_pos + rect.pos, clamped_bounds);
		}
	}
}

bool Widget::SetRectReq(Obj* param) {
	return ((Widget*)param)->TransformRequest();
}

void Widget::RectMod(Obj* param, ModType type) {
	((Widget*)param)->Transform();
}


GUI::GUI(Obj* prnt, TUI* _tui) : UI(prnt) {
	RegisterType("GUI");

	ADDOBJ(ObList, Windows, *this, (this)).Assign("Widget", true);

	Obj& Trigers = ADDOBJ(Obj, Trigers, *this, (this));

	ADDOBJ(CompareExpr, Activate, Trigers, (&Trigers));
	ADDOBJ(CompareExpr, Comfirm, Trigers, (&Trigers));
	ADDOBJ(CompareExpr, Discard, Trigers, (&Trigers));

	tui = _tui;
}


void gui_clear_active(Widget* widget) {

	widget->active = false;

	for (auto child_iter : GETOBJ(ObList, widget, Childs).GetList()) {
		Widget* child = ((Widget*)child_iter.Data());
		gui_clear_active(child);
	}
}

void gui_mark_active(Widget* widget, vec2<float> crs) {

	if (widget->IsActive(crs)) {
		widget->active = true;

		for (auto child_iter : GETOBJ(ObList, widget, Childs).GetList()) {
			Widget* child = ((Widget*)child_iter.Data());
			gui_mark_active(child, crs - child->rect.pos);
		}
	}
}

bool gui_mark_forsed_active(Widget* widget) {

	Bool& need_update = GETOBJ(Bool, widget, Need Update);

	if (GETOBJ(Bool, widget, Forse Active).GetVal() || need_update.GetVal()) {
		widget->active = true;
		need_update.Set(false);
	}

	for (auto child_iter : *widget->childs) {
		Widget* child = ((Widget*)child_iter.Data());
		widget->active = widget->active || gui_mark_forsed_active(child);
	}

	return widget->active;
}

void gui_append_active(List<Widget>* active_widgets, Widget* widget, vec2<float> crs) {

	if (widget->active) {

		widget->local_crs = crs;

		widget->childs->Sort([](const Obj& ob1, const Obj& ob2) {
			return GETOBJ(Int, ((Widget*)&ob1), ProcOrder).GetVal() < GETOBJ(Int, ((Widget*)&ob2), ProcOrder).GetVal();
		});

		for (auto child_iter : *widget->childs) {
			Widget* child = ((Widget*)child_iter.Data());
			gui_append_active(active_widgets, child, crs - child->rect.pos);
		}

		active_widgets->PushBack(widget);
	}
}

void GUI::PumpRequests(ObList* requests) {
	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	
	vec2<float> crs;
	canvas.GetCrsr(crs);
	triggers.Update(&GETOBJ(Obj, this, Trigers));

	List<Widget> active_widgets(false);
	for (auto guii : windows) {
		Widget* window = ((Widget*)guii.Data());

		// mark all active widgets
		gui_mark_forsed_active(window);
		gui_mark_active(window, crs - window->rect.pos);
		
		// Construct prioritized list
		gui_append_active(&active_widgets, window, crs - window->rect.pos);

		// clear active flags
		gui_clear_active(window);
	}

	// process all
	for (auto widget_iter : active_widgets) {
		Widget* widget = ((Widget*)widget_iter.Data());
		widget->Proc(requests, &triggers, tui);
	}
}

void GUI::OutPut(Obj* root) {

	canvas.BeginFrame();

	vec2<float> crs;
	canvas.GetCrsr(crs);

	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	for (auto guii : windows) {
		Widget* window = ((Widget*)guii.Data());
		window->Draw(canvas, vec2<float>(), window->rect);
	}

	canvas.EndFrame();
}
