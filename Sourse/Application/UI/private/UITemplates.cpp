
#include "UI/UITemplates.h"
#include "Core/Seance.h"
#include "Object.h"
#include "Parser.h"
#include "UI/UInputs.h"
#include "UI/UInputsMap.h"
#include "UI/UInterface.h"

typedef struct UIRegionData {
  Operator* op = nullptr;
  Object* RS_ptr = nullptr;
} UIRegionData;

void region_proc(UIItem* This, Seance* C, vec2<SCR_UINT>& cursor) {

  UInputs* user_inputs = C->ui.kmap->uinputs;
  List<OpThread>* threads = &C->threads;

  UIRegionData* rd = (UIRegionData*)This->CustomData;

  if (rd->RS_ptr) {

    threads->add(NEW_DBG(OpThread) OpThread(rd->op, OpEvState::EXECUTE, nullptr));

  } else {

    FOREACH_NODE(Object, (&C->objects), obj_node) {
      if (obj_node->Data->GetRenderComponent()) {
        rd->RS_ptr = obj_node->Data;
        rd->op->Props.Pointers_Buff[0]->assign((void*)This->buff);
        rd->op->Props.Pointers_Obj[0]->assign(rd->RS_ptr);
      }
    }
  }
}

void ui_add_region(UIItem* region, List<Operator>* operators) {

  region->ProcBody = region_proc;

  Operator* target = find_op(operators, &Str("Render To Buff"));

  UIRegionData* rd = NEW_DBG(UIRegionData) UIRegionData();
  region->CustomData = (void*)rd;
  rd->op = target;
}


// --------- Button ---------------- //

typedef struct Button {
  Operator* target = nullptr;
  OpThread* thread;

  bool onpress = false;
  COLOR col_in;
  COLOR col_out;
  COLOR col_hold;

  OpArg pressed;
  OpArg hold;
  OpArg released;

  bool drawhold = false;
} Button;

void button_proc(UIItem* This, Seance* C, vec2<SCR_UINT>& cursor) {

  UInputs* uinpts = C->ui.kmap->uinputs;
  List<OpThread>* queue = &C->threads;

  Button* btn = (Button*)This->CustomData;

  if (btn->thread && btn->thread->state != ThreadState::RUNNING) {
    DELETE_DBG(OpThread, btn->thread);
    btn->thread = nullptr;
    btn->drawhold = false;
  }

  if (uinpts->LMB.state == InputState::PRESSED) {

    if (btn->onpress) {
      btn->thread = NEW_DBG(OpThread) OpThread(btn->target, OpEvState::INVOKE, &btn->pressed);
      queue->add(btn->thread);
      btn->drawhold = true;
    }

  } else if (uinpts->LMB.state == InputState::RELEASED) {

    if (!btn->onpress) {
      btn->thread = NEW_DBG(OpThread) OpThread(btn->target, OpEvState::INVOKE, &btn->released);
      queue->add(btn->thread);
      btn->drawhold = true;

    } else if (btn->thread) {
      btn->thread->modalevent = &btn->released;
    }

  } else if (uinpts->LMB.state == InputState::HOLD && btn->thread) {
    btn->thread->modalevent = &btn->hold;
  }
}

void button_draw(UIItem* This, UIItem* project_to) {
  Button* btn = (Button*)This->CustomData;

  RGBA_32 color1 = 0xffffffff;

  if (btn->drawhold) {
    color1 = btn->col_hold;

  } else if (This->state == UIIstate::LEAVED || This->state == UIIstate::NONE) {
    color1 = btn->col_in;

  } else {
    color1 = btn->col_out;
  }

  Rect<SCR_UINT> rect;
  rect.pos.assign(This->rect.pos.x, This->rect.pos.y);
  rect.size.assign(This->rect.size.x, This->rect.size.y);
  project_to->buff->DrawRect(rect, color1);
}

void ui_template_button(UIItem* button, List<Operator>* operators, DataBlock* db) {

  button->DrawBody = button_draw;
  button->ProcBody = button_proc;
  button->ownbuff = false;

  Button* btn = NEW_DBG(Button) Button();
  btn->target = find_op(operators, &db->find("Operator")->string);
  btn->onpress = db->find("On")->string == "PRESSED";

  DataBlock* argsdb = db->find("Args");
  btn->released.idname = argsdb->find("Pressed")->string;
  btn->hold.idname = argsdb->find("Hold")->string;
  btn->released.idname = argsdb->find("Released")->string;

  DataBlock* palleteb = db->find("Pallete");
  btn->col_out = palleteb->find("In")->integer;
  btn->col_in = palleteb->find("Out")->integer;
  btn->col_hold = palleteb->find("Hold")->integer;

  button->CustomData = btn;
}

// ---------  Group ---------------- //

typedef struct Group {

  COLOR Framein;
  COLOR Frameout;
  COLOR Fillin;
  COLOR Fillout;

  bool frame = true;
  bool fill = false;
  int thickin;
  int thickout;
} Group;

void group_draw(UIItem* This, UIItem* project_to) {

  Group* grp = (Group*)This->CustomData;

  RGBA_32 fillcol = grp->Fillin;
  RGBA_32 framecol = grp->Framein;
  short thick = grp->thickin;

  if (This->state == UIIstate::LEAVED || This->state == UIIstate::NONE) {
    fillcol = grp->Fillout;
    framecol = grp->Frameout;
    thick = grp->thickout;
  }
  
  Rect<SCR_UINT> rect;
  rect.pos.assign(This->rect.pos.x, This->rect.pos.y);
  rect.size.assign(This->rect.size.x, This->rect.size.y);

  if (grp->fill) {
    project_to->buff->DrawRect(rect, fillcol);
  }

  if (grp->frame) {
    project_to->buff->DrawBounds(rect, framecol, thick);
  }

}

void ui_template_group(UIItem* uii, DataBlock* db) {

  uii->DrawBody = group_draw;

  Group* grp = NEW_DBG(Group) Group();
  uii->CustomData = grp;

  grp->frame = db->find("Frame")->boolean;
  grp->fill = db->find("Fill")->boolean;

  if (uii->ownbuff = db->find("OwnBuff")->boolean) {
    uii->buff = NEW_DBG(FBuff<RGBA_32>) FBuff<RGBA_32>(uii->rect.size.x, uii->rect.size.y);
  }

  DataBlock* thickness = db->find("Thickness");
  grp->thickin = thickness->find("In")->integer;
  grp->thickout = thickness->find("Out")->integer;

  DataBlock* pallete = db->find("Pallete");
  grp->Framein = pallete->find("FrameIn")->integer;
  grp->Frameout = pallete->find("FrameOut")->integer;
  grp->Fillin = pallete->find("FillIn")->integer;
  grp->Fillout = pallete->find("FillOut")->integer;
}
