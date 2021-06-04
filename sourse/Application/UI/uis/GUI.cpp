
#include "UI/uis/GUI.h"


#define NVGCOL(col) nvgRGBA(col.r * 255, col.g * 255, col.b * 255, col.a * 255)

WidgetCanvas::WidgetCanvas(class NVGcontext* _vg) {
	vg = _vg;
}

void WidgetCanvas::SetBounds(const Rect<float>& _wrld_rec) {
	wrld_rec = _wrld_rec;
}

void WidgetCanvas::DrawRect(const Rect<float>& _rect, const Color& col, float radius) {

	nvgBeginPath(vg);

	if (!radius) {
		nvgRect(vg, wrld_rec.pos.x, wrld_rec.pos.y, wrld_rec.size.x, wrld_rec.size.y);
	}
	else {
		nvgRoundedRect(vg, wrld_rec.pos.x, wrld_rec.pos.y, wrld_rec.size.x, wrld_rec.size.y, radius);
	}

	nvgFillColor(vg, NVGCOL(col));
	nvgFill(vg);
}

void WidgetCanvas::DrawText(const char* str, const float x, float y, float font_scale, const Color& col) {

	nvgFontSize(vg, font_scale);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, NVGCOL(col));
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgText(vg, x + wrld_rec.pos.x, y + wrld_rec.pos.y, str, NULL);
}

void WidgetCanvas::DrawBounds(const Rect<float>& _rect, const Color& col, short thickness) {

	nvgBeginPath(vg);
	nvgRect(vg, 122, 122, 122, 122);
	nvgCircle(vg, 120, 120, 30);
	nvgPathWinding(vg, NVG_HOLE);	// Mark circle as a hole.
	nvgFillColor(vg, nvgRGBA(255, 192, 0, 100));
	nvgFill(vg);

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));;
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgText(vg, 120, 120, "sdasdasdasdasdasdasd", NULL);
}

void WidgetCanvas::DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness) {
}

void WidgetCanvas::Clear(const Color& col) {
	DrawRect(wrld_rec, col);
}

WidgetCanvas::~WidgetCanvas() {
}



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

		FOREACH_OBJ(childs, guii) {
			((Widget*)guii.Data())->Proc(requests, trigers, crs - rect.pos);
		}

	}
}

void Widget::Draw(WidgetCanvas& canvas, vec2<float> prnt_pos) {

	rect.pos += prnt_pos;
	canvas.SetBounds(rect);
	rect.pos -= prnt_pos;

	DrawBody(canvas);

	FOREACH_OBJ(childs, guii) {
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



GUI::GUI(Obj* prnt, Device* _dev) : UI(prnt) {
	ADDOBJ(ObList, Windows, *this, (this)).Assign("Widget", true);

	Obj& Trigers = ADDOBJ(Obj, Trigers, *this, (this));

	ADDOBJ(CompareExpr, Activate, Trigers, (&Trigers));
	ADDOBJ(CompareExpr, Close, Trigers, (&Trigers));

	dev = _dev;


	canvas = new WidgetCanvas(_dev->getNVGcontext());
}

void GUI::PumpRequests(ObList* requests) {
	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	Obj* trigers = &GETOBJ(Obj, this, Trigers);

	vec2<float> crs;
	dev->GetCrsr(crs);

	FOREACH_OBJ(&windows, guii) {
		Widget* window = ((Widget*)guii.Data());
		window->Proc(requests, trigers, crs - window->rect.pos);
	}
}

void GUI::OutPut(Obj* root) {

	dev->StartDraw();

	List<Obj>& windows = GETOBJ(ObList, this, Windows).GetList();
	FOREACH_OBJ(&windows, guii) {
		((Widget*)guii.Data())->Draw(*canvas, vec2<float>());
	}

	dev->EndDraw();
}
