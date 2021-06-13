
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

	int childs_start = 50;
	int child_height = 30;

	void ProcBody(ObList* requests, vec2<float> crs) {
		if (state == WidgetState::CONFIRM) {

			Link* target = &GETOBJ(Link, this, Target);
			
			Obj* obj = target->GetLink();
			
			if (obj) {
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
		}
	}

	void DrawBody(Window& canvas, vec2<float> crs) {
		if (state != WidgetState::NONE) {
			canvas.DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.13, 0.13, 0.13, 1), 7);
		}
		else {
			canvas.DrawRect(Rect<int>(0, 0, rect.size.x, rect.size.y), Color(0.1, 0.1, 0.1, 1), 7);
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
					canvas.DrawRect(Rect<int>(10, y_pos, rect.size.x - 20, child_height), Color(0.4, 0.4, 0.4, 0.3), 5);
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

		bool known_type = false;

		if (obj->type.IsPrnt("ObList")) {
			int list_offset = offset + 50;
			ObList* list = ((ObList*)obj);

			canvas.Text((Str("List type : ") += list->ListType()).str, 20, list_offset, 16, Color(0.7, 0.7, 0.7, 1));
			list_offset += 30;

			for (auto list_item : list->GetList()) {
				canvas.Text(list_item->type.idname.str, 20, list_offset, 16, Color(0.7, 0.7, 0.7, 1));
				list_offset += child_height;
			}

			known_type = true;
		}
		else if (obj->type.IsPrnt("Int")) {
			known_type = true;
		}

		if (known_type) {
			canvas.Text("Type Data: ", 10, offset += 20, 17, Color(0.7, 0.7, 0.7, 1));
			offset += 30;
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