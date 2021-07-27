
#pragma once

#include "UI/UI.h"
#include "UI/Requester.h"

#include "Primitives.h"

class ShortCut : public Requester {

	ShortCut& operator = (const ShortCut& in);
	ShortCut(const ShortCut& in);

public:

	ShortCut(Obj* prnt);

	virtual ShortCut& Instance() {
		return *new ShortCut(*this);
	}

	void ProcInputs(ObList* reqs);
};

class KeyInput : public Obj {

	KeyInput& operator = (const KeyInput& in);
	KeyInput(const KeyInput& in);

public:

	KeyInput(Obj* prnt);

	virtual KeyInput& Instance() {
		return *new KeyInput(*this);
	}

	void Update(Keyboard* Device);
};

class TUI : public UI {

	TUI& operator = (const TUI& in);
	TUI(const TUI& in);

	Str text_input = "";

public:

	TUI(Obj* prnt);

	virtual TUI& Instance() {
		return *new TUI(*this);
	}

	Keyboard kb;

	void UpdateInputStates();
	void PumpRequests(ObList* requests);
	const Str* get_text_input();

	~TUI();
};