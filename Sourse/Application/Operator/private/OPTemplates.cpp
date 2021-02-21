
#include "Operator/OPTemplates.h"

#include "Operator/Operator.h"
#include "Data/Seance.h"

#include "UI/UInputs.h"
#include "UI/UInputsMap.h"
#include "UI/UInterface.h"

#include "Platform/SysHandler.h"

#include "Types.h"

#include <stdlib.h> // TreadSleap, Exit


class OpEndSeance : Operator {

  void Invoke(struct Seance* C) { Execute(C); }

  void Execute(struct Seance* C) {
    DEL(Seance, C);

    if (LogHeap()) {
      exit(0);
    }
    TreadSleep(10000);

    exit(0);
  }

  bool Poll(struct Seance* C) { return true; }

 public:
  OpEndSeance() { id = "End Seance"; }
};

class OpMoveCanvas : Operator {

  UIItem* target = nullptr;

  void Invoke(struct Seance* C) {
    state = OpState::RUNNING_MODAL;
  }

  void Modal(Seance* C, OpArg* arg) {
    if (arg && arg->idname == "FINISH") {
      state = OpState::FINISHED;
    }

    vec2<float> del;
    del.assign(C->ui.kmap->uinputs->Cdelta.x, C->ui.kmap->uinputs->Cdelta.y);

    FOREACH(&target->hrchy.childs, UIItem, node) { 
      node->move(node->rect.pos + del);
    }    
  }

  bool Poll(struct Seance* C) { 
    target = C->ui.UIroot->active_lower();
    return target;
  }

 public:
  OpMoveCanvas() { 
    id = "Move Canvas";
    rtargs.PushBack(NEW(OpArg)("FINISH"));
  }
};

class OpUIIMove : Operator {

  vec2<float> startpos;
  vec2<float> startcrs;
  UIItem* target = nullptr;

  void Invoke(struct Seance* C) {
    startcrs.assign(C->ui.kmap->uinputs->Cursor.x, C->ui.kmap->uinputs->Cursor.y);
    startpos.assign(target->rect.pos.x, target->rect.pos.y);
    state = OpState::RUNNING_MODAL;
  }

  void Execute(struct Seance* C) { state = OpState::FINISHED; }

  void Modal(Seance* C, OpArg* arg) {
    if (arg && arg->idname == "FINISH") {

      Rect<float> rec = target->hrchy.prnt->rect;

      target->inv_pos.x = (target->rect.pos.x > rec.size.x / 2);
      target->inv_pos.y = (target->rect.pos.y + target->rect.size.y > rec.size.y / 2);

      state = OpState::FINISHED;
    }

    vec2<float> crs;
    crs.assign(C->ui.kmap->uinputs->Cursor.x, C->ui.kmap->uinputs->Cursor.y);
    vec2<float> delta = crs - startcrs;

    vec2<float> pos = startpos + delta;
    target->move(pos);
  }

  bool Poll(struct Seance* C) {
    target = C->ui.UIroot->active_lower();
    return target && target->hrchy.prnt;
  }

 public:
  OpUIIMove() {
    id = "Move UIItem";
    rtargs.PushBack(NEW(OpArg)("FINISH"));
  }
};

class OpUIIResize : Operator {

  bool top = false;
  bool right = false;
  bool bottom = false;
  bool left = false;

  UIItem* target = nullptr;
  Rect<float> startrec;
  vec2<float> startcrs;

  void Invoke(struct Seance* C) {

    target = C->ui.UIroot->active_lower();

    startcrs.assign(C->ui.kmap->uinputs->Cursor.x, C->ui.kmap->uinputs->Cursor.y);
    startrec = target->rect;

    vec2<float> crsr = startcrs;
    vec2<float> wrldpos;
    target->PosInParent(nullptr, wrldpos);
    crsr -= wrldpos;
    float fracx = target->rect.size.x / 3.f;
    float fracy = target->rect.size.y / 3.f;

    top = crsr.y > fracy * 2.f;
    right = crsr.x > fracx * 2.f;
    bottom = crsr.y < fracy;
    left = crsr.x < fracx;

    state = OpState::RUNNING_MODAL;
  }

  void Modal(Seance* C, OpArg* arg) {
    if (arg && arg->idname == "FINISH") {
      state = OpState::FINISHED;
      return;
    }

    SCR_INT dx = C->ui.kmap->uinputs->Cursor.x - (SCR_INT)startcrs.x;
    SCR_INT dy = C->ui.kmap->uinputs->Cursor.y - (SCR_INT)startcrs.y;

    Rect<float> rect(startrec);
    rect.size.y += dy * top;
    rect.size.x += dx * right;

    if (bottom) {
      rect.pos.y += dy;
      rect.size.y -= dy;
    }

    if (left) {
      rect.pos.x += dx;
      rect.size.x -= dx;
    }

    target->Resize(rect);
  }

  bool Poll(struct Seance* C) {
    target = C->ui.UIroot->active_lower();
    return target && target->hrchy.prnt;
  }

 public:
  OpUIIResize() {
    id = "Resize UIItem";
    rtargs.PushBack(NEW(OpArg)("FINISH"));
  }
};

class OpLogHeap : Operator {

  void Invoke(struct Seance* C) { Execute(C); }

  void Execute(struct Seance* C) {
    LogHeap();
    state = OpState::FINISHED;
  }

  bool Poll(struct Seance* C) { return true; }

 public:
  OpLogHeap() { id = "Log Heap"; }
};


class OpConsoleToggle : Operator {

  void Execute(struct Seance* C) {
    C->ui.sysh->ConsoleToggle();
    state = OpState::FINISHED;
  }

  bool Poll(struct Seance* C) { return true; }

 public:
  OpConsoleToggle() { id = "Toggle Console"; }
};

Operators::Operators() {
  ops.PushBack((Operator*)NEW(OpEndSeance)());
  ops.PushBack((Operator*)NEW(OpUIIMove)());
  ops.PushBack((Operator*)NEW(OpUIIResize)());
  ops.PushBack((Operator*)NEW(OpLogHeap)());
  ops.PushBack((Operator*)NEW(OpConsoleToggle)());
  ops.PushBack((Operator*)NEW(OpMoveCanvas)());
}

Operator* Operators::find(Str* id) {
  Operator* target = nullptr;
  Range bnds = Range(0, id->len());
  FOREACH(&ops, Operator, node) {
    if (node->id.match(bnds, *id, bnds)) {
      target = node.Data();
      break;
    }
  }
  return target;
}

Operators::~Operators() {
}