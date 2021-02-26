#pragma once

#include "Containers/List.h"
#include "UI/UInterface.h"
#include "UI/UInputs.h"
#include "IO/Parser.h"
#include "Data/Seance.h"

/*
struct Wrap {
  class UIItem* rig = nullptr;
  class UIItem* top = nullptr;
  class UIItem* lef = nullptr;
  class UIItem* bot = nullptr;
};

struct ResizeData {
  vec2<float> minsize;
  vec2<bool> inv_pos;
  Wrap wrap;
};
*/

bool valid(UIItem *uii, bool dir) {

  FOREACH((&uii->hrchy.childs), UIItem, mainnode) {

    if (uii->recursive_trunsform) {
      if (!mainnode->rect.enclosed_in(uii->rect, true)) {
        return false;
      }
    }

    /*
    if (mainnode->minsize[dir] >= mainnode->rect.size[dir]) {
      return false;
    }
    */

    FOREACH((&uii->hrchy.childs), UIItem, comparenode) {
      if (mainnode == comparenode) {
        continue;
      }
      if (mainnode->rect.overlap(comparenode->rect)) {
        return false;
      }
    }
  }

  return true;
}

/*
void ResizeBody(UIItem* This, Rect<float>& out, bool dir) {

  Rect<float>* prnt_p_rec = &hrchy.prnt->prev_rect;
  Rect<float>* prnt_rec = &hrchy.prnt->rect;
  float bounds[4];
  UIItem* UIrigid;
  float* bnds;

  float fac[2];

  if (!rigid[dir]) {

    for (char offset = 0; offset <= 2; offset += 2) {

      UIrigid = OFFSET(wrap.rig, offset + dir);
      bnds = bounds + offset;

      while (UIrigid) {
        DOIF(break, UIrigid->rigid[dir]);
        UIrigid = OFFSET(UIrigid->wrap.rig, dir + offset);
      }

      bool vanish = (bool)offset;

      if (UIrigid && UIrigid->rigid[dir] && !UIrigid->hide) {
        bnds[0] = UIrigid->rect.pos[dir] + (UIrigid->rect.size[dir] * vanish);
        bnds[1] = UIrigid->prev_rect.pos[dir] + (UIrigid->prev_rect.size[dir] * vanish);

      } else {
        bnds[0] = prnt_rec->size[dir] * !vanish;
        bnds[1] = prnt_p_rec->size[dir] * !vanish;
      }
    }

    for (char plus = 0; plus <= 1; plus++) {
      fac[!plus] = ((bounds + 2)[!plus] - bounds[plus]) / ((bounds + 2)[1] - bounds[1]);
    }

    out.pos[dir] -= bounds[3];
    float pls_width = (out.size[dir] + out.pos[dir]) * fac[1];
    out.pos[dir] *= fac[1];
    out.size[dir] = pls_width - out.pos[dir];
    out.pos[dir] += bounds[3];

    float d1 = ((bounds + 2)[0] - (out.pos[dir] + out.size[dir])) * fac[0];
    float pos = (bounds + 2)[0] - ((bounds + 2)[0] - out.pos[dir]) * fac[0];
    out.size[dir] = (bounds + 2)[0] - out.pos[dir] - d1;
    out.pos[dir] = pos;

  } else if (inv_pos[dir]) {
    out.pos[dir] += prnt_rec->size[dir] - hrchy.prnt->prev_rect.size[dir];
  }
}


void UIItem::update_neighbors(bool recursive) {


  FOREACH(&hrchy.childs, UIItem, cld_node) {
    UIItem& cld = *cld_node.Data();

    cld_node->flag = 0;
    for (char i = 0; i < 4; i++) {
      OFFSET(cld_node->wrap.rig, i) = nullptr;
    }

    if (cld.wrap.top && cld.wrap.bot && cld.wrap.lef && cld.wrap.rig) {
      continue;
    }

    float dist_t = FLT_MAX;
    float dist_b = FLT_MAX;
    float dist_l = FLT_MAX;
    float dist_r = FLT_MAX;

    FOREACH(&hrchy.childs, UIItem, ui_node) {

      if (ui_node == cld_node) {
        continue;
      }

      Rect<float>& rec = ui_node->rect;

      if (cld.rect.intersect_y(rec)) {

        float dist = rec.pos.y - (cld.rect.pos.y + cld.rect.size.y);

        if (!cld.wrap.top && cld.rect.above(rec) && dist_t > dist) {
          cld.wrap.top = ui_node.Data();
          dist_t = dist;
        } else {

          dist = cld.rect.pos.y - (rec.pos.y + rec.size.y);

          if (!cld.wrap.bot && cld.rect.bellow(rec) && dist_b > dist) {
            dist_b = cld.rect.pos.y - rec.pos.y + rec.size.y;
            cld.wrap.bot = ui_node.Data();
            dist_b = dist;
          }
        }

      } else if (cld.rect.intersect_x(rec)) {

        float dist = rec.pos.x - (cld.rect.pos.x + cld.rect.size.x);

        if (!cld.wrap.rig && cld.rect.right(rec) && dist_r > dist) {
          cld.wrap.rig = ui_node.Data();
          dist_r = dist;

        } else {

          dist = cld.rect.pos.x - (rec.pos.x + rec.size.x);

          if (!cld.wrap.lef && cld.rect.left(rec) && dist_l > dist) {
            cld.wrap.lef = ui_node.Data();
            dist_l = dist;
          }
        }
      }

      if (cld.wrap.top && cld.wrap.bot && cld.wrap.lef && cld.wrap.rig) {
        break;
      }
    }
  }

  if (recursive) {
    FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->update_neighbors(recursive); }
  }
}
*/

class UIIButton : UIItem {

  Operator* target = nullptr;
  OpThread* thread = nullptr;

  bool onpress = false;
  COLOR col_in;
  COLOR col_out;
  COLOR col_hold;

  OpArg pressed;
  OpArg hold;
  OpArg released;

  bool drawhold = false;

 public:
  void DrawBody(UIItem* This, UIItem* draw_to) {
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

  void ProcBody(UIItem* This, Seance* C, vec2<SCR_INT>& loc_cursor) {
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

  UIIButton(Operators* ops, DataBlock* paramsdb, DataBlock* uiidb) : UIItem(uiidb) {

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

  bool Valid(bool dir) { return valid(this, dir); }

  ~UIIButton() {}
};

class UIIGroup : UIItem {

  COLOR Framein;
  COLOR Frameout;
  COLOR Fillin;
  COLOR Fillout;

  bool frame = true;
  bool fill = false;
  int thickin;
  int thickout;

 public:
  UIIGroup(Operators* ops, DataBlock* paramsdb, DataBlock* uiidb) : UIItem(uiidb) {

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

  void DrawBody(UIItem* This, UIItem* draw_to) {
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

  bool Valid(bool dir) {
    return valid(this, dir);
  }

  ~UIIGroup() {}
};