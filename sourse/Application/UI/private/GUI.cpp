
#include "UI/GUI.h"

Widget::Widget(const Widget& in) : Requester(in) {
	*childs = GETOBJ(ObList, this, Childs).GetList();
}

Widget::Widget(Obj* prnt, Rect<float> _rect) : Requester(prnt) {
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
}

void Widget::Proc(ObList* requests, Obj* trigers, vec2<float> crs) {

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

		for (auto guii : *childs) {
			((Widget*)guii.Data())->Proc(requests, trigers, crs - rect.pos);
		}

	}
}

void Widget::Draw(Window& canvas, vec2<float> prnt_pos) {

	rect.pos += prnt_pos;
	canvas.SetBounds(rect);
	rect.pos -= prnt_pos;

	DrawBody(canvas);

	for (auto guii : *childs) {
		((Widget*)guii.Data())->Draw(canvas, prnt_pos + rect.pos);
	}

	redraw = false;
}

bool Widget::SetRectReq(Obj* param) {
	return ((Widget*)param)->TransformRequest();
}

void Widget::RectMod(Obj* param, ModType type) {
	((Widget*)param)->Transform();
}



GUI::GUI(Obj* prnt) : UI(prnt) {
	ADDOBJ(ObList, Windows, *this, (this)).Assign("Widget", true);

	Obj& Trigers = ADDOBJ(Obj, Trigers, *this, (this));

	ADDOBJ(CompareExpr, Activate, Trigers, (&Trigers));
	ADDOBJ(CompareExpr, Close, Trigers, (&Trigers));
}

void GUI::PumpRequests(ObList* requests) {
	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	Obj* trigers = &GETOBJ(Obj, this, Trigers);

	vec2<float> crs;
	canvas.GetCrsr(crs);

	for (auto guii : windows) {
		Widget* window = ((Widget*)guii.Data());
		window->Proc(requests, trigers, crs - window->rect.pos);
	}
}

void GUI::OutPut(Obj* root) {

	canvas.BeginFrame();

	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	for (auto guii : windows) {
		((Widget*)guii.Data())->Draw(canvas, vec2<float>());
	}

	canvas.EndFrame();
}
