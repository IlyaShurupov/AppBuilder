
#pragma once

#include "UI/GUI.h"

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

	}

	virtual ContextMenu& Instance() {
		return *new ContextMenu(*this);
	}

	void ProcBody(ObList* requests) {
	}

	void DrawBody(Window& canvas) {
		if (state == WidgetState::INSIDE) {
			canvas.DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.1, 0.1, 0.1, 1), 7);
		}
		else {
			canvas.DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.13, 0.13, 0.13, 1), 7);
		}

		canvas.DrawText("Context Menu", 10, 10, 20, Color(0.7, 0.7, 0.7, 1));

		Obj* obj = GETOBJ(Link, this, Target).GetLink();

		if (obj) {


			int offset = 50;

			MAP_FOREACH(obj->props) {
				MAP_FOREACH_GET(Str, Obj*, obj->props, child) {
					canvas.DrawText(child->key.str, 10, offset, 16, Color(0.7, 0.7, 0.7, 1));
					offset += 30;
				} 
			}
		}
	}

	static void TargetChanged(Obj* ths, ModType type) {
		//GETOBJ(ObList, ths, Childs).;
		//GETOBJ(Link, obj_props, Target).SetLink();
	}

	bool TransformRequest() {
		return false;
	}

	void Transform() {}

	virtual ~ContextMenu() {}
};