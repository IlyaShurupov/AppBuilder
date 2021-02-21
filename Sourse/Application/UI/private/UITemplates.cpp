
#include "UI/UITemplates.h"
#include "Data/Seance.h"
#include "IO/Parser.h"
#include "Object.h"
#include "Operator/OPTemplates.h"
#include "UI/UInputs.h"
#include "UI/UInputsMap.h"
#include "UI/UInterface.h"

UIIButton::UIIButton(Operators* ops, DataBlock* paramsdb, DataBlock* uiidb) : UIItem(uiidb) {

  ownbuff = false;

  target = ops->find(&paramsdb->find("Operator")->string);
  onpress = paramsdb->find("On")->string == "PRESSED";

  DataBlock* argsdb = paramsdb->find("Args");
  released.idname = argsdb->find("Pressed")->string;
  hold.idname = argsdb->find("Hold")->string;
  released.idname = argsdb->find("Released")->string;

  DataBlock* palleteb = paramsdb->find("Pallete");
  col_out = (int)palleteb->find("In")->integer;
  col_in = (int)palleteb->find("Out")->integer;
  col_hold = (int)palleteb->find("Hold")->integer;
}

UIIButton::~UIIButton() {}

void UIIButton::ProcBody(UIItem* This, Seance* C, vec2<SCR_INT>& loc_cursor) {
  UInputs* uinpts = C->ui.kmap->uinputs;
  List<OpThread>* queue = &C->threads;

  if (thread && thread->state != ThreadState::RUNNING) {
    DEL(OpThread, thread);
    thread = nullptr;
    drawhold = false;
  }

  if (uinpts->LMB.state == InputState::PRESSED) {

    if (onpress) {
      thread = NEW(OpThread)(target, OpEvState::INVOKE, &pressed);
      queue->PushBack(thread);
      drawhold = true;
    }

  } else if (uinpts->LMB.state == InputState::RELEASED) {

    if (!onpress) {
      thread = NEW(OpThread)(target, OpEvState::INVOKE, &released);
      queue->PushBack(thread);
      drawhold = true;

    } else if (thread) {
      thread->modalevent = &released;
    }

  } else if (uinpts->LMB.state == InputState::HOLD && thread) {
    thread->modalevent = &hold;
  }
}

void UIIButton::DrawBody(UIItem* This, UIItem* draw_to) {
  RGBA_32 color1 = 0xffffffff;

  if (drawhold) {
    color1 = col_hold;

  } else if (This->state == UIIstate::LEAVED || This->state == UIIstate::NONE) {
    color1 = col_in;

  } else {
    color1 = col_out;
  }

  vec2<float> save(This->rect.pos);
  vec2<float> draw_to_pos;
  This->PosInParent(draw_to, draw_to_pos);
  This->rect.pos = draw_to_pos;

  Rect<float> projectrect;
  This->hrchy.prnt->rect.intersection(This->rect, projectrect);

  Rect<SCR_INT> rect;
  rect.pos.assign(projectrect.pos.x, projectrect.pos.y);
  rect.size.assign(projectrect.size.x, projectrect.size.y);

  draw_to->buff->DrawRect(rect, color1);

  This->rect.pos = save;
}

UIIGroup::UIIGroup(Operators* ops, DataBlock* paramsdb, DataBlock* uiidb) : UIItem(uiidb) {

  frame = paramsdb->find("Frame")->boolean;
  fill = paramsdb->find("Fill")->boolean;

  if (ownbuff = paramsdb->find("OwnBuff")->boolean) {
    buff = NEW(BitMap<RGBA_32>)(rect.size.x, rect.size.y);
  }

  DataBlock* thickness = paramsdb->find("Thickness");
  thickin = (int)thickness->find("In")->integer;
  thickout = (int)thickness->find("Out")->integer;

  DataBlock* pallete = paramsdb->find("Pallete");
  Framein = (int)pallete->find("FrameIn")->integer;
  Frameout = (int)pallete->find("FrameOut")->integer;
  Fillin = (int)pallete->find("FillIn")->integer;
  Fillout = (int)pallete->find("FillOut")->integer;
}

UIIGroup::~UIIGroup() {}

void UIIGroup::ProcBody(UIItem* This, Seance* C, vec2<SCR_INT>& loc_cursor) {}

void UIIGroup::DrawBody(UIItem* This, UIItem* draw_to) {
  vec2<float> save(This->rect.pos);
  vec2<float> draw_to_pos;
  This->PosInParent(draw_to, draw_to_pos);
  This->rect.pos = draw_to_pos;

  RGBA_32 fillcol = Fillin;
  RGBA_32 framecol = Framein;
  short thick = thickin;

  if (This->state == UIIstate::LEAVED || This->state == UIIstate::NONE) {
    fillcol = Fillout;
    framecol = Frameout;
    thick = thickout;
  }

  Rect<float> projectrect(This->rect);
  // This->hrchy.prnt->rect.intersection(This->rect, projectrect);

  Rect<SCR_INT> rect;
  rect.pos.assign(projectrect.pos.x, projectrect.pos.y);
  rect.size.assign(projectrect.size.x, projectrect.size.y);

  if (fill) {
    draw_to->buff->DrawRect(rect, fillcol);
  }

  if (frame) {
    draw_to->buff->DrawBounds(rect, framecol, thick);
  }

  This->rect.pos = save;
}