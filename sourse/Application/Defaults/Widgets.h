
#pragma once

#include "UI/GUI.h"


class ListMenu : public Widget {

	ListMenu& operator = (const ListMenu& in);
	ListMenu(const ListMenu& in) : Widget(in) {
	}

public:

	ListMenu(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
		RegisterType(ObjType("ListMenu"));

		Int* active = &ADDOBJ(Int, Active, *this, (this));
		active->Set(-1);

		Link* target = &ADDOBJ(Link, Target, *this, (this));
		target->Init("ObList", true);
	}

	virtual ListMenu& Instance() {
		return *new ListMenu(*this);
	}

	int items_start = 40;
	int item_size = 25;

	void ProcBody(ObList* requests, vec2<float> crs) {
		ObList* list = (ObList*)GETOBJ(Link, this, Target).GetLink();
		if (!list) {
			return;
		}

		if (state == WidgetState::CONFIRM) {
			int idx = (crs.y - items_start) / item_size;

			if (idx > -1 && idx < list->GetList().Len()) {
				GETOBJ(Int, this, Active).Set(idx);
			} 
			else {
				GETOBJ(Int, this, Active).Set(-1);
			}
		}
		else {
			GETOBJ(Int, this, Active).Set(-1);
		}
	}

	void DrawBody(Window& cnv, vec2<float> crs) {
		if (state != WidgetState::NONE) {
			cnv.RRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.13, 0.13, 0.13, .9), 7);
		}
		else {
			cnv.RRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.1, 0.1, 0.1, .9), 7);
		}

		ObList* list = (ObList*)GETOBJ(Link, this, Target).GetLink();
		if (!list) {
			cnv.Text(" List Object is not specified ", 10, items_start, 16, Color(0.7, 0.7, 0.7, 1));
			return;
		}

		cnv.Text((Str("List Type : ") += list->ListType()).str, 10, 10, 16, Color(0.7, 0.7, 0.7, 1));

		int idx_p = (crs.y - items_start) / item_size;
		if (crs.x > 5 && crs.x < rect.size.x && crs.y > items_start && idx_p > -1 && idx_p < list->GetList().Len()) {
			cnv.RRect(Rect<float>(5, items_start + item_size * idx_p, rect.size.x - 10, item_size), Color(0.4, 0.4, 0.4, 0.3), 5);
		}

		int offset = items_start;
		for (auto item : list->GetList()) {
			cnv.Text(item->type.idname.str, 15, offset + 6, 14, Color(0.7, 0.7, 0.7, 1));
			offset += item_size;
		}
	}

	void Transform() {}

	virtual ~ListMenu() {}
};

class ContextMenu : public Widget {

	ContextMenu& operator = (const ContextMenu& in);
	ContextMenu(const ContextMenu& in) : Widget(in) {
	}

public:

	ContextMenu(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
		RegisterType(ObjType("ContextMenu"));

		Link* target = &ADDOBJ(Link, Target, *this, (this));
		target->Init("Obj", true);
		target->OnModCallBacks.PushBack(OnModCallBack(this, TargetChanged));

		list_menu = new ListMenu(this, Rect<float>(10, 300, 280, 190));
		GETOBJ(Bool, list_menu, Hiden).Set(true);
		GETOBJ(ObList, this, Childs).AddObj(list_menu);
	}

	virtual ContextMenu& Instance() {
		return *new ContextMenu(*this);
	}

	ListMenu* list_menu;
	int childs_start = 50;
	int child_height = 30;

	void ProcBody(ObList* requests, vec2<float> crs) {
		Link* target = &GETOBJ(Link, this, Target);
		Obj* obj = target->GetLink();

		if (!obj) {
			return;
		}
			
		if (state == WidgetState::CONFIRM) {
			
			int offset = childs_start;

			int idx = (crs.y - childs_start) / child_height;
			if (idx == obj->props.nentries && obj->prnt) {
				if (obj->prnt) {
					target->SetLink(obj->prnt);
				}
			} 
			else if (idx >= 0) {
				for (auto child : obj->props) {
					if (child.entry_idx == idx) {
						target->SetLink(child->val);
					}
				}
			}
		}


		if (!GETOBJ(Bool, list_menu, Hiden).GetVal()) {
			int active = GETOBJ(Int, list_menu, Active).GetVal();
			if (active != -1) {
				target->SetLink(&((ObList*)obj)->GetList()[active]);
				GETOBJ(Int, list_menu, Active).Set(-1);
			}
		}
	}

	void DrawBody(Window& canvas, vec2<float> crs) {
		if (state != WidgetState::NONE) {
			canvas.RRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.13, 0.13, 0.13, .9), 7);
		}
		else {
			canvas.RRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.1, 0.1, 0.1, .9), 7);
		}

		canvas.Text("Context Menu : ", 10, 10, 20, Color(0.7, 0.7, 0.7, 1));

		Obj* obj = GETOBJ(Link, this, Target).GetLink();

		if (!obj) {
			return;
		}

		canvas.Text(obj->type.idname.str, 150, 10, 20, Color(0.7, 0.7, 0.7, 1));

		int offset = childs_start;

		if (state == WidgetState::INSIDE) {
			int idx = (crs.y - childs_start) / child_height;
			if (idx <= obj->props.nentries && idx >= 0) {
				if (idx < obj->props.nentries || obj->prnt) {
					int y_pos = childs_start + child_height * idx;
					canvas.RRect(Rect<int>(10, y_pos, rect.size.x - 20, child_height), Color(0.4, 0.4, 0.4, 0.3), 5);
				}
			}
		}

		for (auto chld : obj->props) {
			canvas.Text(chld->key.str, 20, offset + 7, 16, Color(0.7, 0.7, 0.7, 1));
			offset += child_height;
		}

		if (obj->prnt) {
			canvas.Text("Back", 20, offset + 7, 16, Color(0.7, 0.7, 0.7, 1));
			offset += child_height;
		}
	}

	static void TargetChanged(Obj* ths, ModType type) {
		ContextMenu* contex_menu = (ContextMenu*)ths;
		Obj* obj = GETOBJ(Link, contex_menu, Target).GetLink();

		GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(true);

		if (obj->type.IsPrnt("ObList")) {
			GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(false);
			GETOBJ(Link, contex_menu->list_menu, Target).SetLink(obj);
		}
		else if (obj->type.IsPrnt("Int")) {
		}
	}

	bool TransformRequest() {
		return false;
	}

	void Transform() {}

	virtual ~ContextMenu() {}
};