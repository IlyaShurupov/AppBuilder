
#pragma once

#include "UI/GUI.h"

class Label : public Widget {

	Label& operator = (const Label& in);
	Label(const Label& in) : Widget(in) {
	}

public:

	virtual Label& Instance() {
		return *new Label(*this);
	}

	String* text;

	Label(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
		RegisterType(ObjType("Label"));

		text = &ADDOBJ(String, Text, *this, (this));
		text->Assign("Label");

		ADDOBJ(ColorObj, Col, *this, (this)).Set(Color(0.7f, 0.7f, 0.7f, 1.f));
	}

	void DrawBody(Window& cnv, vec2<float> crs) {
		Color text_col;
		GETOBJ(ColorObj, this, Col).Get(&text_col);

		cnv.Text(GETOBJ(String, this, Text).GetStr().str, 5, 5, 16, text_col);
	}

	virtual ~Label() {}
};

class Button : public Widget {

	Button& operator = (const Button& in);
	Button(const Button& in) : Widget(in) {
	}

public:

	virtual Button& Instance() {
		return *new Button(*this);
	}

	Button(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
		RegisterType(ObjType("Button"));

		ADDOBJ(ColorObj, Inactive Col, *this, (this)).Set(Color(0.2f, 0.2f, 0.2f, .9f));
		ADDOBJ(ColorObj, Active Col, *this, (this)).Set(Color(0.23f, 0.23f, 0.23f, .9f));
		ADDOBJ(ColorObj, Activate Col, *this, (this)).Set(Color(0.4f, 0.4f, 0.4f, 0.4f));

		label = new Label(this, Rect<float>(2, 2, rect.size.x - 2, rect.size.y - 2));
		GETOBJ(ObList, this, Childs).AddObj(label);
	}

	Label* label;

	void ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers) {
		if (state == WidgetState::CONFIRM) {
			CreateRequest(requests);
		}
	}

	void DrawBody(Window& cnv, vec2<float> crs) {
		if (state == WidgetState::ACTIVE) {
			Color activate_col;
			GETOBJ(ColorObj, this, Activate Col).Get(&activate_col);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), activate_col, 7);
		}
		else if (state != WidgetState::NONE) {
			Color active_col;
			GETOBJ(ColorObj, this, Active Col).Get(&active_col);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), active_col, 7);
		}
		else {
			Color inactive_col;
			GETOBJ(ColorObj, this, Inactive Col).Get(&inactive_col);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), inactive_col, 7);
		}
	}

	virtual ~Button() {}
};


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

		ADDOBJ(Link, Target, *this, (this)).Init("Widget", true);

		GETOBJ(Int, this, DrawOrder).Set(INT_MAX);
		GETOBJ(Int, this, ProcOrder).Set(INT_MAX);
	}

	vec2<float> prev_crs;
	float scale_factor = 1;
	float content_offset = 0;

	void ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers) {

		Widget* target = (Widget*)GETOBJ(Link, this, Target).GetLink();
		if (!target) {
			return;
		}

		Update();

		bool y_axis = GETOBJ(Bool, this, Vertical).GetVal();

		if (state == WidgetState::ACTIVATE) {
			prev_crs = local_crs;
			GETOBJ(Bool, this, Forse Active).Set(true);
			triggers->handled = true;
		}
		else if (state == WidgetState::ACTIVE) {

			if (scale_factor <= 1) {
				return;
			}

			vec2<float> delta = local_crs - prev_crs;
			float content_offset_delta = (&delta.x)[y_axis] * scale_factor;

			for (auto child : GETOBJ(ObList, target, Childs).GetList()) {
				Widget* widget = (Widget*)child.Data();
				if (widget->type.idname == "Scroller" || GETOBJ(Bool, widget, Hiden).GetVal()) {
					continue;
				}

				(&widget->rect.pos.x)[y_axis] += content_offset_delta;
				widget->ApplyRect();
			}

			triggers->handled = true;

			prev_crs = local_crs;
		}
		else if (state == WidgetState::CONFIRM || state == WidgetState::DISCARD) {
			GETOBJ(Bool, this, Forse Active).Set(false);
			triggers->handled = true;
		}
	}

	void DrawBody(Window& cnv, vec2<float> crs) {

		Widget* target = (Widget*)GETOBJ(Link, this, Target).GetLink();
		if (!target) {
			return;
		}

		if (scale_factor <= 1) {
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

	void Update() {

		Widget* target = (Widget*)GETOBJ(Link, this, Target).GetLink();
		if (!target) {
			return;
		}

		bool y_axis = GETOBJ(Bool, this, Vertical).GetVal();

		// updating rectangle
		rect.pos.x = (target->rect.size.x - 20) * y_axis + 5;
		rect.pos.y = (target->rect.size.y - 20) * (!y_axis) + 5;
		rect.size.x = 20 * (y_axis)+target->rect.size.x * !y_axis - 10;
		rect.size.y = 20 * (!y_axis) + target->rect.size.y * y_axis - 10;

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
		content_offset = min_val;
		scale_factor = content_size / (&target->rect.size.x)[y_axis];
	}

	virtual ~Scroller() {}
};


class Group : public Widget {

	Group& operator = (const Group& in);
	Group(const Group& in) : Widget(in) {
	}

public:

	virtual Group& Instance() {
		return *new Group(*this);
	}

	Scroller* scroller;
	Scroller* scroller_x;

	Group(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
		RegisterType(ObjType("Group"));

		scroller = new Scroller(this);
		GETOBJ(Link, scroller, Target).SetLink(this);
		GETOBJ(ObList, this, Childs).AddObj(scroller);

		scroller_x = new Scroller(this);
		GETOBJ(Link, scroller_x, Target).SetLink(this);
		GETOBJ(Bool, scroller_x, Vertical).Set(false);
		GETOBJ(ObList, this, Childs).AddObj(scroller_x);
	}

	virtual ~Group() {}
};

class Menu : public Widget {

	Menu& operator = (const Menu& in);

public:

	Menu(const Menu& in) : Widget(in) {
	}

	virtual Menu& Instance() {
		return *new Menu(*this);
	}

	Group* topbar;
	Group* body;
	Button* collapse_btn;
	Label* title;

	Menu(Obj* prnt, Rect<float> _rect) : Widget(prnt, _rect) {
		RegisterType(ObjType("Menu"));

		ADDOBJ(ColorObj, Inactive Col, *this, (this)).Set(Color(0.1f, 0.1f, 0.1f, .9f));
		ADDOBJ(ColorObj, Active Col, *this, (this)).Set(Color(0.13f, 0.13f, 0.13f, .9f));

		ADDOBJ(Int, Roundness, *this, (this)).Set(7);
		
		topbar = new Group(this, Rect<float>(40, 5, rect.size.x - 5, 30));
		GETOBJ(ObList, this, Childs).AddObj(topbar);

		body = new Group(this, Rect<float>(5, 40, rect.size.x - 10, rect.size.y - 10 - 35));
		GETOBJ(ObList, this, Childs).AddObj(body);

		ADDOBJ(Bool, Collapsed, *this, (this)).Set(false);

		collapse_btn = new Button(this, Rect<float>(5, 5, 30, 30));
		GETOBJ(ObList, this, Childs).AddObj(collapse_btn);

		title = new Label(this, Rect<float>(0, 5, 60, 30));
		GETOBJ(ObList, topbar, Childs).AddObj(title);
	}

	void ProcBody(ObList* requests, TUI* tui) {
		if (collapse_btn->state == WidgetState::CONFIRM) {
			GETOBJ(Bool, this, Collapsed).Set(!GETOBJ(Bool, this, Collapsed).GetVal());
		}

		if (GETOBJ(Bool, this, Collapsed).GetVal()) {
			rect.size.y = topbar->rect.size.y + topbar->rect.pos.y + 5;
			GETOBJ(Bool, body, Hiden).Set(true);
		}
		else {
			rect.size.y = body->rect.size.y + body->rect.pos.y + 5;
			GETOBJ(Bool, body, Hiden).Set(false);
		}
	}

	void DrawBody(Window& cnv, vec2<float> crs) {

		if (state != WidgetState::NONE) {
			Color active;
			GETOBJ(ColorObj, this, Active Col).Get(&active);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), active, GETOBJ(Int, this, Roundness).GetVal());
		}
		else {
			Color inactive;
			GETOBJ(ColorObj, this, Inactive Col).Get(&inactive);
			cnv.RRect(Rect<float>(0, 0, rect.size.x, rect.size.y), inactive, GETOBJ(Int, this, Roundness).GetVal());
		}
	}

	virtual ~Menu() {}
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

	void ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers);
	void DrawBody(Window& cnv, vec2<float> crs);

	virtual ~InputField() {}
};


class ListMenu : public Menu {

	ListMenu& operator = (const ListMenu& in);
	ListMenu(const ListMenu& in);

	template <typename ContainerType, typename ContainerIterType>
	void proc_by_type() {
		ContainerType* target = (ContainerType*)GETOBJ(Link, this, Target).GetLink();

		ContainerIterType target_list = target->Iterator();
		List<Obj>* widget_list = &GETOBJ(ObList, body, Childs).GetList();

		int widget_items_len = 0;

		for (auto child : *widget_list) {
			if (child->type.IsPrnt(widget_type)) {
				widget_items_len++;
			}
		}

		int diff = target_list.Len() - widget_items_len;
		int len = (diff > 0 ? widget_items_len : target_list.Len());

		Node<Obj>* widget_node = widget_list->First();

		while (widget_node && !widget_node->data->type.IsPrnt(widget_type)) {
			widget_node = widget_node->next;
		}

		for (int idx = 0; idx < len; idx++) {

			update_item((Widget*)widget_node->data, target_list.data(), target_list.name());

			++target_list;
			widget_node = widget_node->next;

			while (widget_node && !widget_node->data->type.IsPrnt(widget_type)) {
				widget_node = widget_node->next;
			}
		}

		if (diff < 0) {
			Node<Obj>* widget_node = widget_list->Last();
			while (diff) {
				if (widget_node->data->type.IsPrnt(widget_type)) {
					Node<Obj>* del_node = widget_node;
					widget_node = widget_node->prev;
					widget_list->DelNode(del_node);
					diff++;
				}
				else {
					widget_node = widget_node->prev;
				}
			}
		}
		else if (diff > 0) {

			float pos = -FLT_MAX;
			if (widget_items_len) {
				for (auto item : *widget_list) {
					if (!item->type.IsPrnt(widget_type)) {
						continue;
					}
					float item_pos = ((Widget*)item.Data())->rect.size_vec_w().y;
					if (item_pos > pos) {
						pos = item_pos;
					}
				}
			}
			else {
				pos = 0;
			}

			pos += 5;

			target_list = target->Iterator(target_list.Len() - diff);

			while (diff) {

				Widget* new_item = append_item(body, Rect<float>(0, pos, body->rect.size.x, 30));
				
				update_item(new_item, target_list.data(), target_list.name());

				GETOBJ(ObList, body, Childs).AddObj(new_item);
				diff--;

				++target_list;

				pos += 35;
			}
		}
	}

public:

	virtual ListMenu& Instance() {
		return *new ListMenu(*this);
	}

	ListMenu(Obj* prnt, Rect<float> _rect);

	float items_start = 40;
	float item_size = 25;

	void ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers);

	static Widget* LabelAppendItem(Obj* parent, const Rect<float>& rect) {
		return new Label(parent, rect);
	}

	static void LabelUpdateItem(Widget* widget, Obj* obj, Str* suggested) {
		((Label*)widget)->text->Assign(suggested ? *suggested : obj->type.idname);
	}

	static Widget* ButtonAppendItem(Obj* parent, const Rect<float>& rect) {
		return new Button(parent, rect);
	}

	static void ButtonUpdateItem(Widget* widget, Obj* obj, Str* suggested) {
		((Button*)widget)->label->text->Assign(suggested ? *suggested : obj->type.idname);
	}


	void (*update_item)(Widget* widget, Obj* obj, Str* suggested) = LabelUpdateItem;
	Widget* (*append_item)(Obj* parent, const Rect<float>& rect) = LabelAppendItem;
	Str widget_type = "Label";

	virtual ~ListMenu() {}
};

class ContextMenu : public Menu {

	ContextMenu& operator = (const ContextMenu& in);
	ContextMenu(const ContextMenu& in);

public:

	virtual ContextMenu& Instance() {
		return *new ContextMenu(*this);
	}

	ContextMenu(Obj* prnt, Rect<float> _rect);

	ObDict* dictlist;
	ListMenu* dict_menu;

	ListMenu* list_menu;
	InputField* input_field;
	Button* back_button;

	void ProcBody(ObList* requests, TUI* tui, WidgetTriggers* triggers);
	void DrawBody(Window& canvas, vec2<float> crs);

	static void TargetChanged(Obj* ths, ModType type);

	virtual ~ContextMenu() {}
};