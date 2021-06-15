
#pragma once

#include "Defaults/Widgets.h"


InputField::InputField(const InputField& in) : Widget(in) {
}

InputField::InputField(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
	RegisterType(ObjType("InputField"));

	ADDOBJ(String, Input, *this, (this));
	ADDOBJ(Link, Target, *this, (this)).Init("Obj", true);
}

void InputField::ProcBody(ObList* requests, TUI* tui, vec2<float> crs) {
	if (state == WidgetState::CONFIRM) {
		if (input) {
			if (!valid_input || (valid_input && valid_input(&GETOBJ(String, this, Input).GetStr()))) {
				GETOBJ(Link, this, Target).GetLink()->from_string(&GETOBJ(String, this, Input).GetStr());
			}

			GETOBJ(Bool, this, Forse Active).Set(false);
			input = false;
		}
		else {
			GETOBJ(String, this, Input).Assign("");
			GETOBJ(Bool, this, Forse Active).Set(true);
			input = true;
		}
	}

	if (input) {
		String* input = &GETOBJ(String, this, Input);
		const Str& tui_input = *tui->get_text_input();
		input->GetStr() += tui_input;
	}
}

void InputField::DrawBody(Window& cnv, vec2<float> crs) {
	if (state != WidgetState::NONE) {
		if (input) {
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), Color(0.17f, 0.17f, 0.17f, .9f), 7);
		}
		else {
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), Color(0.13f, 0.13f, 0.13f, .9f), 7);
		}
	}
	else {
		cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), Color(0.1f, 0.1f, 0.1f, .9f), 7);
	}

	if (input) {
		cnv.Text((Str("Value : ") += GETOBJ(String, this, Input).GetStr()).str, 10, 10, 16, Color(0.7f, 0.7f, 0.7f, 1.f));
	}
	else {
		Obj* obj = GETOBJ(Link, this, Target).GetLink();
		if (!obj) {
			cnv.Text(" No Target Specified", 10, 10, 16, Color(0.7f, 0.7f, 0.7f, 1.f));
			return;
		}

		Str obj_str;
		obj->as_string(&obj_str);

		cnv.Text((Str("Value : ") += obj_str).str, 10, 10, 16, Color(0.7f, 0.7f, 0.7f, 1.f));
	}
}


ListMenu::ListMenu(const ListMenu& in) : Widget(in) {
}

ListMenu::ListMenu(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
	RegisterType(ObjType("ListMenu"));

	Int* active = &ADDOBJ(Int, Active, *this, (this));
	active->Set(-1);

	Link* target = &ADDOBJ(Link, Target, *this, (this));
	target->Init("ObList", true);
}

void ListMenu::ProcBody(ObList* requests, TUI* tui, vec2<float> crs) {
	ObList* list = (ObList*)GETOBJ(Link, this, Target).GetLink();
	if (!list) {
		return;
	}

	if (state == WidgetState::CONFIRM) {
		int idx = (int)((crs.y - items_start) / item_size);

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

void ListMenu::DrawBody(Window& cnv, vec2<float> crs) {
	if (state != WidgetState::NONE) {
		cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), Color(0.13f, 0.13f, 0.13f, .9f), 7);
	}
	else {
		cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), Color(0.1f, 0.1f, 0.1f, .9f), 7);
	}

	ObList* list = (ObList*)GETOBJ(Link, this, Target).GetLink();
	if (!list) {
		cnv.Text(" List Object is not specified ", 10, items_start, 16, Color(0.7f, 0.7f, 0.7f, 1.f));
		return;
	}

	cnv.Text((Str("List Type : ") += list->ListType()).str, 10, 10, 16, Color(0.7f, 0.7f, 0.7f, 1.f));

	int idx_p = (int)((crs.y - items_start) / item_size);
	if (crs.x > 5 && crs.x < rect.size.x && crs.y > items_start && idx_p > -1 && idx_p < list->GetList().Len()) {
		cnv.RRect(Rect<float>(5, items_start + item_size * idx_p, rect.size.x - 10, item_size), Color(0.4f, 0.4f, 0.4f, 0.3f), 5);
	}

	float offset = items_start;
	for (auto item : list->GetList()) {
		cnv.Text(item->type.idname.str, 15, offset + 6, 14, Color(0.7f, 0.7f, 0.7f, 1.f));
		offset += item_size;
	}
}



ContextMenu::ContextMenu(const ContextMenu& in) : Widget(in) {
}

ContextMenu::ContextMenu(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
	RegisterType(ObjType("ContextMenu"));

	Link* target = &ADDOBJ(Link, Target, *this, (this));
	target->Init("Obj", true);
	target->OnModCallBacks.PushBack(OnModCallBack(this, TargetChanged));

	list_menu = new ListMenu(this, Rect<float>(10, 300, 280, 190));
	GETOBJ(Bool, list_menu, Hiden).Set(true);
	GETOBJ(ObList, this, Childs).AddObj(list_menu);

	input_field = new InputField(this, Rect<float>(10, 100, 280, 30));
	GETOBJ(Bool, input_field, Hiden).Set(true);
	GETOBJ(ObList, this, Childs).AddObj(input_field);
}

void ContextMenu::ProcBody(ObList* requests, TUI* tui, vec2<float> crs) {
	Link* target = &GETOBJ(Link, this, Target);
	Obj* obj = target->GetLink();

	if (!obj) {
		return;
	}

	if (state == WidgetState::CONFIRM) {

		float offset = childs_start;

		int idx = (int)((crs.y - childs_start) / child_height);
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

void ContextMenu::DrawBody(Window& canvas, vec2<float> crs) {
	if (state != WidgetState::NONE) {
		canvas.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), Color(0.13f, 0.13f, 0.13f, .9f), 7);
	}
	else {
		canvas.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), Color(0.1f, 0.1f, 0.1f, .9f), 7);
	}

	canvas.Text("Context Menu : ", 10, 10, 20, Color(0.7f, 0.7f, 0.7f, 1.f));

	Obj* obj = GETOBJ(Link, this, Target).GetLink();

	if (!obj) {
		return;
	}

	canvas.Text(obj->type.idname.str, 150, 10, 20, Color(0.7f, 0.7f, 0.7f, 1.f));

	float offset = childs_start;

	if (state == WidgetState::INSIDE) {
		int idx = (int)((crs.y - childs_start) / child_height);
		if (idx <= obj->props.nentries && idx >= 0) {
			if (idx < obj->props.nentries || obj->prnt) {
				float y_pos = childs_start + child_height * idx;
				canvas.RRect(Rect<float>(10, y_pos, rect.size.x - 20, child_height), Color(0.4f, 0.4f, 0.4f, 0.3f), 5);
			}
		}
	}

	for (auto chld : obj->props) {
		canvas.Text(chld->key.str, 20, offset + 7, 16, Color(0.7f, 0.7f, 0.7f, 1.f));
		offset += child_height;
	}

	if (obj->prnt) {
		canvas.Text("Back", 20, offset + 7, 16, Color(0.7f, 0.7f, 0.7f, 1.f));
		offset += child_height;
	}
}

void ContextMenu::TargetChanged(Obj* ths, ModType type) {
	ContextMenu* contex_menu = (ContextMenu*)ths;
	Obj* obj = GETOBJ(Link, contex_menu, Target).GetLink();

	GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(true);
	GETOBJ(Bool, contex_menu->input_field, Hiden).Set(true);
	contex_menu->input_field->valid_input = nullptr;

	if (obj->type.IsPrnt("ObList")) {
		GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(false);
		GETOBJ(Link, contex_menu->list_menu, Target).SetLink(obj);
	}
	else if (obj->type.IsPrnt("Int") || obj->type.IsPrnt("Float") || obj->type.IsPrnt("Bool") || obj->type.IsPrnt("String")) {
		GETOBJ(Bool, contex_menu->input_field, Hiden).Set(false);
		GETOBJ(Link, contex_menu->input_field, Target).SetLink(obj);
	}
}
