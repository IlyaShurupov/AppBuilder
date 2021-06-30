
#include "UI/GUI.h"

Widget::Widget(const Widget& in) : Requester(in) {
	*childs = GETOBJ(ObList, this, Childs).GetList();
}

Widget::Widget(Obj* prnt, Rect<float> _rect) : Requester(prnt) {
	RegisterType(ObjType("Widget"));

	ADDOBJ(ObList, Childs, *this, (this)).Assign("Widget", true);
	childs = &GETOBJ(ObList, this, Childs).GetList();


	Obj& rect_obj = ADDOBJ(Obj, Rect, *this, (this));
	ADDOBJ(Float, Pos X, rect_obj, (&rect_obj)).Set(_rect.pos.x);
	ADDOBJ(Float, Pos Y, rect_obj, (&rect_obj)).Set(_rect.pos.y);
	ADDOBJ(Float, Size X, rect_obj, (&rect_obj)).Assign(_rect.size.x, 5, 2000);
	ADDOBJ(Float, Size Y, rect_obj, (&rect_obj)).Assign(_rect.size.y, 5, 2000);

	//rect_obj.BindModPoll(this, SetRectReq);
	//rect_obj.AddOnModCallBack(this, RectMod);

	rect = _rect;

	ADDOBJ(Bool, Hiden, *this, (this)).Set(false);
	ADDOBJ(Bool, Forse Active, *this, (this)).Set(false);

	ADDOBJ(Int, DrawOrder, *this, (this));
}

void Widget::Proc(ObList* requests, Obj* trigers, TUI* tui, vec2<float> crs) {
	
	UpdateRect();

	if (crs.x > 0 && crs.y > 0 && rect.size > crs || active) {

		bool activate_action = GETOBJ(CompareExpr, trigers, Activate).Evaluate();
		bool comfirm_action = GETOBJ(CompareExpr, trigers, Comfirm).Evaluate();
		bool discard_action = GETOBJ(CompareExpr, trigers, Discard).Evaluate();

		if (activate_action) {
			state = WidgetState::ACTIVATE;
		}
		else if (comfirm_action) {
			state = WidgetState::CONFIRM;
		}
		else if (discard_action) {
			state = WidgetState::DISCARD;
		}
		else if (state == WidgetState::NONE) {
			state = WidgetState::ENTERED;
		}
		else if (state == WidgetState::ACTIVATE) {
			state = WidgetState::ACTIVE;
		}
		else if (state == WidgetState::ACTIVE) {
		}
		else {
			state = WidgetState::INSIDE;
		}

		active = true;

	}
	else {

		if (state == WidgetState::NONE) {
			return;
		}
		else if (state == WidgetState::LEAVED) {
			state = WidgetState::NONE;
			active = true;
		}
		else {
			state = WidgetState::LEAVED;
			active = true;
		}
	}

	if (active) {

		ProcBody(requests, tui, crs);

		for (auto guii : *childs) {
			Widget* child = (Widget*)guii.Data();
			if (!GETOBJ(Bool, child, Hiden).GetVal()) {

				if (child->skip_iteration) {
					child->skip_iteration = false;
					continue;
				}

				child->Proc(requests, trigers, tui, crs - child->rect.pos);
			}
		}

	}

	ApplyRect();
}

void Widget::Draw(Window& canvas, vec2<float> prnt_pos, vec2<float> crs, const Rect<float>& draw_bounds) {

	UpdateRect();

	rect.pos += prnt_pos;
	canvas.SetCanvasRect(rect);

	Rect<float> clamped_bounds(draw_bounds);

	if (prnt && prnt->type.IsPrnt("Widget")) {
		Rect<float> prnt_rect(prnt_pos, ((Widget*)prnt)->rect.size);

		if (!prnt_rect.overlap(rect)) {
			rect.pos -= prnt_pos;
			active = false;
			return;
		}

		clamped_bounds.clamp(prnt_rect);
	}

	canvas.SetBounds(clamped_bounds);
	rect.pos -= prnt_pos;

	DrawBody(canvas, crs);

	childs->Sort([](const Obj& ob1, const Obj& ob2) {
		return GETOBJ(Int, ((Widget*)&ob1), DrawOrder).GetVal() > GETOBJ(Int, ((Widget*)&ob2), DrawOrder).GetVal();
	});

	for (auto guii : *childs) {
		Widget* child = (Widget*)guii.Data();
		if (!GETOBJ(Bool, child, Hiden).GetVal()) {
			child->Draw(canvas, prnt_pos + rect.pos, crs - child->rect.pos, clamped_bounds);
		}
	}

	active = false;
}

bool Widget::SetRectReq(Obj* param) {
	return ((Widget*)param)->TransformRequest();
}

void Widget::RectMod(Obj* param, ModType type) {
	((Widget*)param)->Transform();
}


GUI::GUI(Obj* prnt, TUI* _tui) : UI(prnt) {
	RegisterType(ObjType("GUI"));

	ADDOBJ(ObList, Windows, *this, (this)).Assign("Widget", true);

	Obj& Trigers = ADDOBJ(Obj, Trigers, *this, (this));

	ADDOBJ(CompareExpr, Activate, Trigers, (&Trigers));
	ADDOBJ(CompareExpr, Comfirm, Trigers, (&Trigers));
	ADDOBJ(CompareExpr, Discard, Trigers, (&Trigers));

	tui = _tui;
}

void gui_find_forsed_active(List<Obj>* forsed_active, Widget* widget) {
	for (auto child_iter : GETOBJ(ObList, widget, Childs).GetList()) {
		Widget* child = ((Widget*)child_iter.Data());
		if (GETOBJ(Bool, child, Forse Active).GetVal()) {
			forsed_active->PushBack(child);
		}
		gui_find_forsed_active(forsed_active, child);
	}
}

void GUI::PumpRequests(ObList* requests) {
	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	Obj* trigers = &GETOBJ(Obj, this, Trigers);

	vec2<float> crs;
	canvas.GetCrsr(crs);

	List<Obj> forsed_active(false);
	for (auto guii : windows) {
		Widget* window = ((Widget*)guii.Data());
		gui_find_forsed_active(&forsed_active, window);
	}
	

	for (auto guii : forsed_active) {
		Widget* widget = ((Widget*)guii.Data());
		while (widget) {
			widget->active = true;
			
			if (!widget->prnt->type.IsPrnt("Widget")) {
				break;
			}

			widget = (Widget*)widget->prnt;
		}
	}

	for (auto guii : windows) {
		Widget* window = ((Widget*)guii.Data());
		window->Proc(requests, trigers, tui, crs - window->rect.pos);
	}
}

void GUI::OutPut(Obj* root) {

	canvas.BeginFrame();

	vec2<float> crs;
	canvas.GetCrsr(crs);

	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	for (auto guii : windows) {
		Widget* window = ((Widget*)guii.Data());
		window->Draw(canvas, vec2<float>(), crs - window->rect.pos, window->rect);
	}

	canvas.EndFrame();
}
