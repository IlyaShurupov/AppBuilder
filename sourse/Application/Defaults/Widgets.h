
#pragma once

#include "UI/GUI.h"

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