
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

void InputField::ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers) {
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

		cnv.Text((Str("Value : ") += obj->as_string()).str, 10, 10, 16, text_col);
	}
}


ListMenu::ListMenu(const ListMenu& in) : Menu(in) {
}

ListMenu::ListMenu(Obj* prnt, Rect<float> _rect) : Menu(prnt, _rect) {
	RegisterType(ObjType("ListMenu"));

	Link* target = &ADDOBJ(Link, Target, *this, (this));
	target->Init("Obj", true);
}

void ListMenu::ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers) {

	Menu::ProcBody(requests, tui, triggers);

	Obj* container_obj = GETOBJ(Link, this, Target).GetLink();
	
	if (!container_obj) {
		return;
	}

	if (container_obj->type.IsPrnt("ObDict")) {
		target_is_self = false;
		proc_by_type<ObDict, ObDictIter>();
	}
	else if (container_obj->type.IsPrnt("ObList")) {
		if (container_obj == &GETOBJ(ObList, this->body, Childs)) {
			target_is_self = true;

			Node<Obj>* widget_node = body->childs->Last();

			while (widget_node) {

				if (widget_node->data->type.IsPrnt(widget_type)) {
					Node<Obj>* del_node = widget_node;
					widget_node = widget_node->prev;
					body->childs->DelNode(del_node);
				}
				else {
					widget_node = widget_node->prev;
				}
			}
			return;
		}
		target_is_self = false;
		proc_by_type<ObList, ObListIter>();
	}
}


ContextMenu::ContextMenu(const ContextMenu& in) : Menu(in) {
}

ContextMenu::ContextMenu(Obj* prnt, Rect<float> _rect, OpHolder* copy_op, Obj* copy_dest) : Menu(prnt, _rect) {
	RegisterType(ObjType("ContextMenu"));

	Link* target = &ADDOBJ(Link, Target, *this, (this));
	target->Init("Obj", true);
	target->OnModCallBacks.PushBack(OnModCallBack(this, TargetChanged));

	{
		list_menu = new ListMenu(body, Rect<float>(0, 315, body->rect.size.x, 200));
	
		list_menu->append_item = ListMenu::ButtonAppendItem;
		list_menu->update_item = ListMenu::ButtonUpdateItem;
		list_menu->widget_type = "Button";
	
		GETOBJ(ObList, body, Childs).AddObj(list_menu);
	}

	{
		dictlist = new ObDict(nullptr);
		dictlist->SetRef(nullptr, true);
		
		dict_menu = new ListMenu(body, Rect<float>(0, 0, body->rect.size.x, 200));
		
		dict_menu->append_item = ListMenu::ButtonAppendItem;
		dict_menu->update_item = ListMenu::ButtonUpdateItem;
		dict_menu->widget_type = "Button";

		GETOBJ(Link, dict_menu, Target).SetLink(dictlist);
		GETOBJ(ObList, body, Childs).AddObj(dict_menu);
	}

	{
		input_field = new InputField(body, Rect<float>(0, 315, body->rect.size.x, 30));
		GETOBJ(ObList, body, Childs).AddObj(input_field);
	}

	{
		link_menu = new LinkMenu(body, Rect<float>(0, 315, body->rect.size.x, 200));
		GETOBJ(ObList, body, Childs).AddObj(link_menu);
	}

	back_button = new Button(dict_menu->topbar, Rect<float>(100, 0, 60, 25));
	GETOBJ(ObList, dict_menu->topbar, Childs).AddObj(back_button);

	{
		copy_button = new Button(body, Rect<float>(0, 210, body->rect.size.x, 30));
		GETOBJ(Link, copy_button, Target Op).SetLink(copy_op);
	
		ObDict& op_args = GETOBJ(ObDict, copy_button, Op Args);
		((Link&)op_args.GetObj("Destination")).SetLink(copy_dest);

		GETOBJ(ObList, body, Childs).AddObj(copy_button);
	}

	{
		paste_button = new Button(body, Rect<float>(0, 245, body->rect.size.x, 30));
		GETOBJ(Link, paste_button, Target Op).SetLink(copy_op);

		ObDict& op_args = GETOBJ(ObDict, copy_button, Op Args);
		((Link&)op_args.GetObj("Target")).SetLink(&copy_dest->GetChld("Clipboard Object"));

		GETOBJ(ObList, body, Childs).AddObj(paste_button);
	}

	copy_link_button = new Button(body, Rect<float>(0, 280, body->rect.size.x, 30));
	GETOBJ(ObList, body, Childs).AddObj(copy_link_button);
}

void ContextMenu::update_content() {
	GETOBJ(Bool, dict_menu, Need Update).Set(true);
	GETOBJ(Bool, list_menu, Need Update).Set(true);

	GETOBJ(Bool, dict_menu->body->scroller, Need Update).Set(true);
	GETOBJ(Bool, list_menu->body->scroller, Need Update).Set(true);
}

void ContextMenu::ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers) {

	Menu::ProcBody(requests, tui, triggers);

	Link* target = &GETOBJ(Link, this, Target);
	Obj* obj = target->GetLink();

	if (!obj) {
		return;
	}

	if (!GETOBJ(Bool, list_menu, Hiden).GetVal()) {
		int non_type = 0;
		for (auto item : *list_menu->body->childs) {
			if (!item->type.IsPrnt("Button")) {
				non_type++;
				continue;
			}
			if (((Widget*)item.Data())->state == WidgetState::CONFIRM) {
				Obj* new_target = nullptr;
				Obj* target_list = GETOBJ(Link, list_menu, Target).GetLink();

				if (target_list->type.IsPrnt("ObDict")) {
					new_target = ((ObDict*)target_list)->GetDict().GetEntry(item.Idx() - non_type)->val;
				}
				else if (target_list->type.IsPrnt("ObList")) {
					new_target = &((ObList*)target_list)->GetList()[item.Idx() - non_type];
				}

				if (new_target) {
					target->SetLink(new_target);
					((Widget*)item.Data())->state = WidgetState::NONE;
					return;
				}
			}
		}
	} 
	
	if (!GETOBJ(Bool, dict_menu, Hiden).GetVal()) {
		int non_type = 0;
		for (auto item : *dict_menu->body->childs) {
			if (!item->type.IsPrnt("Button")) {
				non_type++;
				continue;
			}
			if (((Widget*)item.Data())->state == WidgetState::CONFIRM) {
				Obj* new_target = obj->props.GetEntry(item.Idx() - non_type)->val;
				target->SetLink(new_target);
				((Widget*)item.Data())->state = WidgetState::NONE;
				return;
			}
		}
	}

	if (back_button->state == WidgetState::CONFIRM) {
		if (obj->prnt) {
			target->SetLink(obj->prnt);
			return;
		}
	}

	if (!GETOBJ(Bool, link_menu, Hiden).GetVal()) {
		
		if (link_menu->select->state == WidgetState::CONFIRM) {
			target->SetLink(((Link*)GETOBJ(Link, link_menu, Target).GetLink())->GetLink());
			link_menu->select->state = WidgetState::NONE;
			return;
		}

		if (link_menu->past->state == WidgetState::CONFIRM) {
			((Link*)GETOBJ(Link, link_menu, Target).GetLink())->SetLink(link_clipboard);
			return;
		}
	}

	if (copy_link_button->state == WidgetState::CONFIRM) {
		link_clipboard = target->GetLink();
		return;
	}


	ObDict& copy_args = GETOBJ(ObDict, copy_button, Op Args);
	Obj* copied = &((Link&)copy_args.GetObj("Destination")).GetLink()->GetChld("Clipboard Object");

	ObDict& paste_args = GETOBJ(ObDict, paste_button, Op Args);
	((Link&)paste_args.GetObj("Target")).SetLink(copied);
}

void ContextMenu::DrawBody(Window& canvas, vec2<float> crs) {

	Menu::DrawBody(canvas, crs);
}

void ContextMenu::TargetChanged(Obj* ths, ModType type) {
	ContextMenu* contex_menu = (ContextMenu*)ths;
	Obj* new_target = GETOBJ(Link, contex_menu, Target).GetLink();

	GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(true);
	GETOBJ(Bool, contex_menu->input_field, Hiden).Set(true);
	GETOBJ(Bool, contex_menu->link_menu, Hiden).Set(true);

	contex_menu->input_field->valid_input = nullptr;

	{
		ObDict& op_args = GETOBJ(ObDict, contex_menu->copy_button, Op Args);
		((Link&)op_args.GetObj("Target")).SetLink(new_target);
	}

	{
		ObDict& op_args = GETOBJ(ObDict, contex_menu->paste_button, Op Args);
		((Link&)op_args.GetObj("Destination")).SetLink(new_target);
	}

	if (new_target->type.IsPrnt("ObList") || new_target->type.IsPrnt("ObDict")) {
		GETOBJ(Bool, contex_menu->list_menu, Hiden).Set(false);
		GETOBJ(Link, contex_menu->list_menu, Target).SetLink(new_target);
	}
	else if (new_target->type.IsPrnt("Int") || new_target->type.IsPrnt("Float") || new_target->type.IsPrnt("Bool") || new_target->type.IsPrnt("String")) {
		GETOBJ(Bool, contex_menu->input_field, Hiden).Set(false);
		GETOBJ(Link, contex_menu->input_field, Target).SetLink(new_target);
	}
	else if (new_target->type.IsPrnt("Link")) {
		GETOBJ(Bool, contex_menu->link_menu, Hiden).Set(false);
		GETOBJ(Link, contex_menu->link_menu, Target).SetLink(new_target);
	}

	contex_menu->title->text->Assign(Str("Context Menu : ") += new_target->type.idname);

	contex_menu->dictlist->SetRef(&new_target->props, false);
	
	contex_menu->update_content();
	//contex_menu->update_content();
}
