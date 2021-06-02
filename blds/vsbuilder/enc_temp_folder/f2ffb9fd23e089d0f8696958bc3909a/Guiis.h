
#pragma once

#include "UI/uis/GUI.h"

class ContextMenu : public Guii {

	ContextMenu& operator = (const ContextMenu& in);
	ContextMenu(const ContextMenu& in) : Guii(in) {
	}

public:

	ContextMenu(Obj* prnt, Rect<float> _rect) : Guii(prnt, _rect) {
		RegisterType(ObjType("ContextMenu"));

		ADDOBJ(Link, Target, *this, (this)).Init("Obj", true);
	}

	virtual ContextMenu& Instance() {
		return *new ContextMenu(*this);
	}

	void ProcBody(ObList* requests) {
		if (state == GuiiState::ACTIVATE) {
			int i = 0;
		}
		else if (state == GuiiState::CLOSE) {
			int i = 0;
		}
		else if (state == GuiiState::ENTERED) {
			int i = 0;
		}
		else if (state == GuiiState::INSIDE) {
			int i = 0;
		}
	}

	void DrawBody(Obj* root_obj) {
		if (state == GuiiState::INSIDE) {
			buff->DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.1, 0.1, 0.1, 1), 7);
		}
		else {
			buff->DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.13, 0.13, 0.13, 1), 7);
		}

		buff->DrawText("Context Menu", 10, 10, 20, Color(0.7, 0.7, 0.7, 1));

		Obj* obj = GETOBJ(Link, this, Target).GetLink();

		if (obj) {
			List<Tuple<Str*, Obj*>> list;

			obj->props.ToList(&list);

			int offset = 50;
			for (Iterator<Tuple<Str*, Obj*>> child(&list, 0); child < list.Len(); ++child) {
				
				buff->DrawText(child->t1->str, 10, offset, 16, Color(0.7, 0.7, 0.7, 1));

				offset += 30;
			}
		}
	}

	bool TransformRequest() {
		return false;
	}

	void Transform() {}

	virtual ~ContextMenu() {}
};