
#pragma once

#include "Defaults/Widgets.h"


InputField::InputField(const InputField& in) : Widget(in) {
}

InputField::InputField(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
	RegisterType(ObjType("InputField"));

	ADDOBJ(String, Input, *this, (this));
	ADDOBJ(Link, Target, *this, (this)).Init("Obj", true);

	ADDOBJ(ColorObj, Inactive Col, *this, (this)).Set(Color(0.1f, 0.1f, 0.1f, .9f));
	ADDOBJ(ColorObj, Active Col, *this, (this)).Set(Color(0.13f, 0.13f, 0.13f, .9f));
	ADDOBJ(ColorObj, Input Col, *this, (this)).Set(Color(0.4f, 0.4f, 0.4f, 0.3f));
	ADDOBJ(ColorObj, Text Col, *this, (this)).Set(Color(0.7f, 0.7f, 0.7f, 1.f));
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
			Color input_col;
			GETOBJ(ColorObj, this, Input Col).Get(&input_col);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), input_col, 7);
		}
		else {
			Color active;
			GETOBJ(ColorObj, this, Active Col).Get(&active);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), active, 7);
		}
	}
	else {
		Color inactive;
		GETOBJ(ColorObj, this, Inactive Col).Get(&inactive);
		cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), inactive, 7);
	}

	Color text_col;
	GETOBJ(ColorObj, this, Text Col).Get(&text_col);

	if (input) {
		cnv.Text((Str("Value : ") += GETOBJ(String, this, Input).GetStr()).str, 10, 10, 16, text_col);
	}
	else {
		Obj* obj = GETOBJ(Link, this, Target).GetLink();
		if (!obj) {
			cnv.Text(" No Target Specified", 10, 10, 16, text_col);
			return;
		}

		Str obj_str;
		obj->as_string(&obj_str);

		cnv.Text((Str("Value : ") += obj_str).str, 10, 10, 16, text_col);
	}
}


ListMenu::ListMenu(const ListMenu& in) : Menu(in) {
}

ListMenu::ListMenu(Obj* prnt, Rect<float> _rect) : Menu(prnt, _rect) {
	RegisterType(ObjType("ListMenu"));

	Int* active = &ADDOBJ(Int, Active, *this, (this));
	active->Set(-1);

	Link* target = &ADDOBJ(Link, Target, *this, (this));
	target->Init("ObList", true);
}

void ListMenu::ProcBody(ObList* requests, TUI* tui, vec2<float> crs) {

	Menu::ProcBody(requests, tui, crs);

	ObList* target = (ObList*)GETOBJ(Link, this, Target).GetLink();
	if (!target) {
		return;
	}

	List<Obj>* target_list = &target->GetList();
	List<Obj>* widget_list = &GETOBJ(ObList, body, Childs).GetList();

	int widget_items_len = 0;
	
	for (auto child : *widget_list) {
		if (child->type.IsPrnt(widget_type)) { 
			widget_items_len++; 
		}
	}

	int diff = target_list->Len() - widget_items_len;
	int len = (diff > 0 ? widget_items_len : target_list->Len());

	Node<Obj>* target_node = target_list->First();
	Node<Obj>* widget_node = widget_list->First();

	while (widget_node && !widget_node->data->type.IsPrnt(widget_type)) {
		widget_node = widget_node->next;
	}

	for (int idx = 0; idx < len; idx ++) {

		update_item((Widget*)widget_node->data, *target_node->data);

		target_node = target_node->next;
		widget_node = widget_node->next;

		while (widget_node && !widget_node->data->type.IsPrnt(widget_type)) {
			widget_node = widget_node->next;
		}
	}

	if (diff < 0) {
		while (diff) {
			widget_list->DelNode(widget_list->Last());
			diff++;
		}
	}
	else if (diff > 0) {

		float pos = widget_list ->Len() ? -FLT_MAX : 0;

		for (auto item : *widget_list) {
			float item_pos = ((Widget*)item.Data())->rect.size_vec_w().y;
			if (item_pos > pos) {
				pos = item_pos;
			}
		}

		pos += 5;

		Node<Obj>* new_target_node = target_list->Find(target_list->Len() - diff);

		while (diff) {

			Widget* new_item = append_item(new_target_node->data, body, Rect<float>(0, pos, body->rect.size.x, 30));
			GETOBJ(ObList, body, Childs).AddObj(new_item);
			diff--;

			new_target_node = new_target_node->next;

			pos += 35;
		}
	}
}


ContextMenu::ContextMenu(const ContextMenu& in) : Widget(in) {
}

ContextMenu::ContextMenu(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
	RegisterType(ObjType("ContextMenu"));

	Link* target = &ADDOBJ(Link, Target, *this, (this));
	target->Init("Obj", true);
	target->OnModCallBacks.PushBack(OnModCallBack(this, TargetChanged));

	list_menu = new ListMenu(this, Rect<float>(5, 300, rect.size.x - 10, 190));
	GETOBJ(Bool, list_menu, Hiden).Set(true);
	GETOBJ(ObList, this, Childs).AddObj(list_menu);

	input_field = new InputField(this, Rect<float>(5, 120, rect.size.x - 10, 30));
	GETOBJ(Bool, input_field, Hiden).Set(true);
	GETOBJ(ObList, this, Childs).AddObj(input_field);

	label = new Label(this, Rect<float>(5, 5, rect.size.x - 10, 50));
	GETOBJ(ObList, this, Childs).AddObj(label);

	back_button = new Button(this, Rect<float>(230, 5, 60, 25));
	GETOBJ(ObList, this, Childs).AddObj(back_button);

	Scroller* scroller = new Scroller(this);
	GETOBJ(Link, scroller, Target).SetLink(this);
	GETOBJ(ObList, this, Childs).AddObj(scroller);

	Scroller* scroller_x = new Scroller(this);
	GETOBJ(Link, scroller_x, Target).SetLink(this);
	GETOBJ(Bool, scroller_x, Vertical).Set(false);
	GETOBJ(ObList, this, Childs).AddObj(scroller_x);

	ADDOBJ(ColorObj, Inactive Col, *this, (this)).Set(Color(0.1f, 0.1f, 0.1f, .9f));
	ADDOBJ(ColorObj, Active Col, *this, (this)).Set(Color(0.13f, 0.13f, 0.13f, .9f));
	ADDOBJ(ColorObj, Active Item Col, *this, (this)).Set(Color(0.4f, 0.4f, 0.4f, 0.3f));
	ADDOBJ(ColorObj, Text Col, *this, (this)).Set(Color(0.7f, 0.7f, 0.7f, 1.f));
}

void ContextMenu::ProcBody(ObList* requests, TUI* tui, vec2<float> crs) {
	Link* target = &GETOBJ(Link, this, Target);
	Obj* obj = target->GetLink();

	if (!obj) {
		return;
	}

	if (back_button->state == WidgetState::CONFIRM) {
		if (obj->prnt) {
			target->SetLink(obj->prnt);
		}
	}

	if (state == WidgetState::CONFIRM) {

		float offset = childs_start;

		int idx = (int)((crs.y - childs_start) / child_height);

		if (idx >= 0) {
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
		Color active;
		GETOBJ(ColorObj, this, Active Col).Get(&active);
		canvas.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), active, 7);
	}
	else {
		Color inactive;
		GETOBJ(ColorObj, this, Inactive Col).Get(&inactive);
		canvas.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), inactive, 7);
	}

	Color text_col;
	GETOBJ(ColorObj, this, Text Col).Get(&text_col);


	Obj* obj = GETOBJ(Link, this, Target).GetLink();

	if (!obj) {
		return;
	}

	float offset = childs_start;

	if (state == WidgetState::INSIDE) {
		int idx = (int)((crs.y - childs_start) / child_height);
		if (idx < obj->props.nentries && idx >= 0) {
			if (idx < obj->props.nentries || obj->prnt) {

				Color acive_item;
				GETOBJ(ColorObj, this, Active Item Col).Get(&acive_item);

				float y_pos = childs_start + child_height * idx;
				canvas.RRect(Rect<float>(10, y_pos, rect.size.x - 20, child_height), acive_item, 5);
			} 
		}
	}

	for (auto chld : obj->props) {
		canvas.Text(chld->key.str, 20, offset + 7, 16, text_col);
		offset += child_height;
	}
}

void ContextMenu::TargetChanged(Obj* ths, ModType type) {
	ContextMenu* contex_menu = (ContextMenu*)ths;
	Obj* new_target = GETOBJ(Link, contex_menu, Target).GetLink();

	GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(true);
	GETOBJ(Bool, contex_menu->input_field, Hiden).Set(true);
	contex_menu->input_field->valid_input = nullptr;

	if (new_target->type.IsPrnt("ObList")) {
		GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(false);
		GETOBJ(Link, contex_menu->list_menu, Target).SetLink(new_target);
		contex_menu->list_menu->skip_iteration = true;
	}
	else if (new_target->type.IsPrnt("Int") || new_target->type.IsPrnt("Float") || new_target->type.IsPrnt("Bool") || new_target->type.IsPrnt("String")) {
		GETOBJ(Bool, contex_menu->input_field, Hiden).Set(false);
		GETOBJ(Link, contex_menu->input_field, Target).SetLink(new_target);
		contex_menu->input_field->skip_iteration = true;
	}

	GETOBJ(String, contex_menu->label, Text).Assign(Str("Context Menu : ") += new_target->type.idname);
}
