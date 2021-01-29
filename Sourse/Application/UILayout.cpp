
#include "public/UILayout.h"
#include "public/Seance.h"
#include "public/KeyMap.h"
#include "Object.h"

void UIItem::ProcEvent(List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {

  UIstate newState;

  if (rect.inside((float)cursor.x, (float)cursor.y)) {

    if (state == UIstate::NONE) {
      newState = UIstate::ENTERED;
    } else {
      newState = UIstate::INSIDE;
      redraw = true;
    }

  } else {
    if (state == UIstate::INSIDE) {
      newState = UIstate::LEAVED;
    } else {
      newState = UIstate::NONE;
    }
  }

  if (state != newState) {
    redraw = true;
    state = newState;

    
  }

  if (state == UIstate::INSIDE) {
    if (ProcBody)
      ProcBody(this, op_threads, user_inputs, cursor, C);
  } 
  
  if (redraw) {
    FOREACH_NODE(UIItem, (&hierarchy.childs), child_node) {
      vec2<SCR_UINT> pos = vec2<SCR_UINT>((SCR_UINT)rect.pos.x, (SCR_UINT)rect.pos.y);
      child_node->Data->ProcEvent(op_threads, user_inputs, (cursor - pos), C);
    }
  }
}

void UIItem::Draw(UIItem* project_to) {

  if (DrawBody)
    DrawBody(this, project_to);

  FOREACH_NODE(UIItem, (&hierarchy.childs), child_node) {

    if (ownbuff) {
      child_node->Data->Draw(this);
      continue;
    }

    child_node->Data->rect.pos += rect.pos;
    child_node->Data->Draw(project_to);
    child_node->Data->rect.pos -= rect.pos;
  }

  if (ownbuff && project_to) {
    buff->project_to(project_to->buff, vec2<SCR_UINT>((SCR_UINT)rect.pos.x, (SCR_UINT)rect.pos.y));
  }

  redraw = false;
}

UIItem::UIItem(vec2<SCR_UINT>* size) {

  state = UIstate::NONE;
  Resize = nullptr;
  ProcBody = nullptr;
  DrawBody = nullptr;

  if (this->ownbuff = (bool)size) {
    buff = DBG_NEW FBuff<RGBA_32>();
    buff->resize(size->x, size->y);
  }
}

UIItem::~UIItem() {
  hierarchy.childs.del();
  if (buff)
    delete buff;
  //if (CustomData) 
    //delete CustomData;
}

// --------- Button ---------------- //

void button_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT> & cursor, Seance* C) {
  if (user_inputs->LMB.state == InputState::RELEASED) {
    op_threads->add(DBG_NEW OpThread((Operator *)This->CustomData, OpEventState::EXECUTE, nullptr));
  }
}

void button_draw(UIItem* This, UIItem* project_to) {

  RGBA_32 color1 = 0xffffffff;
  RGBA_32 color2 = 0xff090909;
  if (This->state == UIstate::LEAVED || This->state == UIstate::NONE) {
    color1 = 0xffaaaaaa;
  }

  Rect<SCR_UINT> rect(This->rect);

  project_to->buff->DrawRect(rect, color1);
  project_to->buff->DrawBounds(rect, color2, 1);
}

void ButtonResize(UIItem* This, vec2<float> &rescale) {

}

UIItem* ui_add_button(UIItem* parent, vec2<SCR_UINT> pos, List<Operator>* operators, Str* op_idname) {

  UIItem* button = DBG_NEW UIItem(nullptr);
  button->hierarchy.join(parent);
  
  button->ownbuff = false;
  button->DrawBody = button_draw;
  button->ProcBody = button_proc;
  button->Resize = ButtonResize;

  button->rect.size.assign(40, 20);
  button->rect.pos.assign((float)pos.x, (float)pos.y);

  //own
  Operator* op_ptr = find_op(operators, op_idname);
  if (!op_ptr) {
    return nullptr;
  }
  button->CustomData = (void*)op_ptr;
  //own

  return button;
}

// -------------------------    ------------------------- //

void UIResize(UIItem* This, vec2<float>& rescale) {

  float width = (This->rect.size.x + This->rect.pos.x) * rescale.x;
  float height = (This->rect.size.y + This->rect.pos.y) * rescale.y;

  This->rect.pos.x *= rescale.x;
  This->rect.pos.y *= rescale.y;

  This->rect.size.x = width - This->rect.pos.x;
  This->rect.size.y = height - This->rect.pos.y;

  
  if (This->ownbuff) {
    This->buff->resize((SCR_UINT)This->rect.size.x, (SCR_UINT)This->rect.size.y);
  }

  FOREACH_NODE(UIItem, (&This->hierarchy.childs), child_node) {
    child_node->Data->Resize(child_node->Data, rescale);
  }
}

// --------- Region ---------------- //

typedef struct UIRegionData {
  Operator* op = nullptr;
  Object* RS_ptr = nullptr;
}UIRegionData;

void region_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT> & cursor, Seance* C) {

  UIRegionData* rd = (UIRegionData *)This->CustomData;

  if (rd->RS_ptr) {

    op_threads->add(DBG_NEW OpThread(rd->op, OpEventState::EXECUTE, nullptr));

  } else {

    FOREACH_NODE(Object, (&C->project.collection), obj_node) {
      if (obj_node->Data->GetRenderComponent()) {
        rd->RS_ptr = obj_node->Data;
        rd->op->Props.Pointers_Buff[0]->assign((void*)This->buff);
        rd->op->Props.Pointers_Obj[0]->assign(rd->RS_ptr);
      }
    }
  }
}

void region_draw(UIItem* This, UIItem* project_to) {

}

UIItem* ui_add_region(UIItem* parent, Rect<SCR_UINT> rect, List<Operator>* operators) {


  UIItem* region = DBG_NEW UIItem(&rect.size);
  region->hierarchy.join(parent);

  region->ownbuff = true;
  region->DrawBody = region_draw;
  region->ProcBody = region_proc;
  region->Resize = UIResize;

  region->rect.size.assign((float)rect.size.x, (float)rect.size.y);
  region->rect.pos.assign((float)rect.pos.x, (float)rect.pos.y);

  // own
  Operator* op_ptr = find_op(operators, &Str("Render To Buff"));
  if (!op_ptr) {
    return nullptr;
  }

  UIRegionData* rd = DBG_NEW UIRegionData();
  region->CustomData = (void*)rd;
  rd->op = op_ptr;
  // own

  return region;
}


// ---------  Area ---------------- //

void area_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT> & cursor, Seance* C) {

}

void area_draw(UIItem* This, UIItem* project_to) {

  RGBA_32 color2 = 0xff050505;
  short thick = 3;

  if (This->state == UIstate::LEAVED || This->state == UIstate::NONE) {
    color2 = 0xff101010;
    thick = 2;
  }

  Rect<SCR_UINT> rect(This->rect);
  project_to->buff->DrawBounds(rect, color2, thick);
}

UIItem* ui_add_area(UIItem* parent, Rect<SCR_UINT> rect, Str name) {

  UIItem* Area = DBG_NEW UIItem(nullptr);
  Area->hierarchy.join(parent);

  Area->ownbuff = false;
  Area->DrawBody = area_draw;
  Area->ProcBody = area_proc;
  Area->Resize = UIResize;
  Area->idname = name;

  Area->rect.size.assign((float)rect.size.x, (float)rect.size.y);
  Area->rect.pos.assign((float)rect.pos.x, (float)rect.pos.y);

  // own

  return Area;
}

// ------------------ UI Root --------------------------------- //

void Uiproc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT> & loc_cursor, Seance* C) {

}

void UIdraw(UIItem* This, UIItem* project_to) {
  RGBA_32 color = 0xff1d1d21;
  This->buff->clear(&color);
}

UIItem* ui_add_root(Rect<SCR_UINT> rect) {

  UIItem* UIroot = DBG_NEW UIItem(&rect.size);
  UIroot->ProcBody = Uiproc;
  UIroot->DrawBody = UIdraw;
  UIroot->Resize = UIResize;

  UIroot->rect.size.assign((float)rect.size.x, (float)rect.size.y);
  UIroot->rect.pos.assign((float)rect.pos.x, (float)rect.pos.y);

  UIroot->minsize.y = 60;
  UIroot->minsize.x = 100;
  UIroot->ownbuff = true;
  return UIroot;
}

// ---------------------- UI compiling -------------------------  //

UIItem* UI_compile(List<Operator>* operators, Str* ui_path, Window* parent) {

  UIItem* UIroot = ui_add_root(Rect<SCR_UINT>(550, 200, 900, 600));

  UIItem* Area = ui_add_area(UIroot, Rect<SCR_UINT>(20, 20, 300, 300), "View3d");

  UIItem* Region = ui_add_region(Area, Rect<SCR_UINT>(20, 20, 200, 200), operators);

  UIItem* Button = ui_add_button(Area, vec2<SCR_UINT>(20, 20), operators, &Str("Add Plane"));

  short width = 25;
  short border = 10;
  Rect<SCR_UINT> rect = Rect<SCR_UINT>(border, (SCR_UINT)UIroot->rect.size.y - width - border, (SCR_UINT)UIroot->rect.size.x - border*2, width);
  UIItem* Area2 = ui_add_area(UIroot, rect, "View3d");

  ui_add_button(Area2, vec2<SCR_UINT>(2, 2), operators, &Str("Toggle Console"));
  ui_add_button(Area2, vec2<SCR_UINT>(2 + 40 * 1, 2), operators, &Str("End Seance"));
  ui_add_button(Area2, vec2<SCR_UINT>(2 + 40 * 2, 2), operators, &Str("Move Window"));
  return UIroot;
}

