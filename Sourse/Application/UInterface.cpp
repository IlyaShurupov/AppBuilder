
#include "public/UInterface.h"
#include "Object.h"
#include "public/Seance.h"
#include "public/UInputsMap.h"
#include "public/FileReader.h"

UIItem::UIItem() {
  flag = 0;
  state = UIIstate::NONE;
  ProcBody = nullptr;
  DrawBody = nullptr;
  buff = nullptr;
}

UIItem::~UIItem() {
  hrchy.childs.del();
  if (buff) {
    DELETE_DBG(FBuff<RGBA_32>, buff);
  }
}

void UIItem::ProcEvent(List<OpThread>* op_threads, struct UInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {

  IF(hide, return );

  UIIstate newState;

  if (rect.inside((float)cursor.x, (float)cursor.y)) {

    if (state == UIIstate::NONE) {
      newState = UIIstate::ENTERED;
    } else {
      newState = UIIstate::INSIDE;
      redraw = true;
    }

  } else {
    if (state == UIIstate::INSIDE) {
      newState = UIIstate::LEAVED;
    } else {
      newState = UIIstate::NONE;
    }
  }

  if (state != newState) {
    redraw = true;
    state = newState;
  }

  if (state == UIIstate::INSIDE) {
    IF(ProcBody, ProcBody(this, op_threads, user_inputs, cursor, C))
  }

  if (redraw) {
    FOREACH_NODE(UIItem, (&hrchy.childs), child_node) {
      vec2<SCR_UINT> pos = vec2<SCR_UINT>((SCR_UINT)rect.pos.x, (SCR_UINT)rect.pos.y);
      child_node->Data->ProcEvent(op_threads, user_inputs, (cursor - pos), C);
    }
  }
}

void UIItem::Draw(UIItem* project_to) {

  IF(hide, return );

  IF(DrawBody, DrawBody(this, project_to));

  FOREACH_NODE(UIItem, (&hrchy.childs), child_node) {

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

void UIItem::Resize(Rect<float>& newrect) {
  update_neighbors(true);
  save_config();
  if (resize_dir(newrect.size.y / rect.size.y, 1)) {
    rect.pos.y = newrect.pos.y;
  }
  if (resize_dir(newrect.size.x / rect.size.x, 0)) {
    rect.pos.x = newrect.pos.x;
  }
  update_buff(true);
}

bool UIItem::valid_resize(Rect<float>& newrec, bool dir) {
  bool pass = true;
  // Hide if overlaped or min size triggered
  if (!rect.enclosed_in(hrchy.prnt->rect, true)) {
    goto DISKARD;
  }

  if (minsize[dir] >= newrec.size[dir]) {
    goto DISKARD;
  }

  FOREACH_NODE(UIItem, (&hrchy.prnt->hrchy.childs), child_node) {
    if (child_node->Data != this && rect.overlap(child_node->Data->rect)) {
      goto DISKARD;
    }
  }

  return true;

 DISKARD:
  rect.size[dir] = prev_rect.size[dir];
  rect.pos[dir] = prev_rect.pos[dir];
  flag = 0;
  return false;
}

bool UIItem::resize_dir(float rescale, bool dir) {

  prev_rect.size[dir] = rect.size[dir];
  prev_rect.pos[dir] = rect.pos[dir];

  if (!hrchy.prnt) {

    rect.size[dir] *= rescale;

  } else {

    ResizeBody(rect, dir);

    IF(!valid_resize(rect, dir), return false);
  }

  // repead recursively
  bool reset = false;
  float chld_rscl = rect.size[dir] / prev_rect.size[dir];

  FOREACH_NODE(UIItem, (&hrchy.childs), child_node) {
    if (child_node->Data->rigid[dir]) {
      if (!child_node->Data->resize_dir(chld_rscl, dir)) {
        reset = true;
        break;
      }
    }
  }

  if (!reset) {
    FOREACH_NODE(UIItem, (&hrchy.childs), child_node) {
      if (!(child_node->Data->rigid[dir])) {
        if (!child_node->Data->resize_dir(chld_rscl, dir)) {
          reset = true;
          break;
        }
      }
    }
  }

  if (reset) {
    resize_discard(dir);
    return false;
  }

  redraw = true;
  flag = 1;
  return true;
}

void UIItem::resize_discard(bool dir) {
  FOREACH_NODE(UIItem, (&hrchy.childs), child_node) {
    if (child_node->Data->flag == 1) {
      child_node->Data->resize_discard(dir);
      child_node->Data->rect.pos[dir] = child_node->Data->prev_rect.pos[dir];
      child_node->Data->rect.size[dir] = child_node->Data->prev_rect.size[dir];
      flag = 0;
    }
  }

  rect.size[dir] = prev_rect.size[dir];
  rect.pos[dir] = prev_rect.pos[dir];
  flag = 0;
}

void UIItem::ResizeBody(Rect<float>& out, bool dir) {

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

UIItem* UIItem::find(Str* string) {

  if (hrchy.id == *string) {
    return this;
  }

  UIItem* item; 

  FOREACH(&hrchy.childs, UIItem, node) {
    if (item = node->Data->find(string)) {
      return item;
    }
  }

  return nullptr;
}

void UIItem::update_neighbors(bool recursive) {


  FOREACH(&hrchy.childs, UIItem, cld_node) {
    UIItem& cld = *cld_node->Data;

    cld_node->Data->flag = 0;
    for (char i = 0; i < 4; i++) {
      OFFSET(cld_node->Data->wrap.rig, i) = nullptr;
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

      Rect<float>& rec = ui_node->Data->rect;

      if (cld.rect.intersect_y(rec)) {

        float dist = rec.pos.y - (cld.rect.pos.y + cld.rect.size.y);

        if (!cld.wrap.top && cld.rect.above(rec) && dist_t > dist) {
          cld.wrap.top = ui_node->Data;
          dist_t = dist;
        } else {

          dist = cld.rect.pos.y - (rec.pos.y + rec.size.y);

          if (!cld.wrap.bot && cld.rect.bellow(rec) && dist_b > dist) {
            dist_b = cld.rect.pos.y - rec.pos.y + rec.size.y;
            cld.wrap.bot = ui_node->Data;
            dist_b = dist;
          }
        }

      } else if (cld.rect.intersect_x(rec)) {

        float dist = rec.pos.x - (cld.rect.pos.x + cld.rect.size.x);

        if (!cld.wrap.rig && cld.rect.right(rec) && dist_r > dist) {
          cld.wrap.rig = ui_node->Data;
          dist_r = dist;

        } else {

          dist = cld.rect.pos.x - (rec.pos.x + rec.size.x);

          if (!cld.wrap.lef && cld.rect.left(rec) && dist_l > dist) {
            cld.wrap.lef = ui_node->Data;
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
    FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->Data->update_neighbors(recursive); }
  }
}

void UIItem::update_buff(bool recursive) {
  IF((ownbuff && !hide), buff->resize((SCR_UINT)rect.size.x, (SCR_UINT)rect.size.y));
  FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->Data->update_buff(recursive); }
}

void UIItem::clear_flags() {
  flag = 0;
  FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->Data->clear_flags(); }
}

void UIItem::save_config() {
  prev_rect = rect;
  FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->Data->save_config(); }
}

// ------------------------------------ User Defined  ------------------------------------------------------------- //

// --------- Button ---------------- //

void button_proc(UIItem* This, List<OpThread>* op_threads, struct UInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {
  if (user_inputs->LMB.state == InputState::RELEASED) {
    op_threads->add(NEW_DBG(OpThread) OpThread((Operator*)This->CustomData, OpEvState::EXECUTE, nullptr));
  }
}

void button_draw(UIItem* This, UIItem* project_to) {

  RGBA_32 color1 = 0xffffffff;
  RGBA_32 color2 = 0xff090909;
  if (This->state == UIIstate::LEAVED || This->state == UIIstate::NONE) {
    color1 = 0xffaaaaaa;
  }

  Rect<SCR_UINT> rect(This->rect);

  project_to->buff->DrawRect(rect, color1);
  project_to->buff->DrawBounds(rect, color2, 1);
}

void ui_add_button(UIItem* button, List<Operator>* operators, Str* op_idname) {
  button->ownbuff = false;
  button->DrawBody = button_draw;
  button->ProcBody = button_proc;
  Operator* target = find_op(operators, op_idname);
  button->CustomData = (void*)target;
}

// --------- Region ---------------- //

typedef struct UIRegionData {
  Operator* op = nullptr;
  Object* RS_ptr = nullptr;
} UIRegionData;

void region_proc(UIItem* This, List<OpThread>* op_threads, struct UInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {

  UIRegionData* rd = (UIRegionData*)This->CustomData;

  if (rd->RS_ptr) {

    op_threads->add(NEW_DBG(OpThread) OpThread(rd->op, OpEvState::EXECUTE, nullptr));

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

void ui_add_region(UIItem* region, List<Operator>* operators) {
 
  region->ownbuff = true;
  region->ProcBody = region_proc;
 
  Operator* target = find_op(operators, &Str("Render To Buff"));

  UIRegionData* rd = NEW_DBG(UIRegionData) UIRegionData();
  region->CustomData = (void*)rd;
  rd->op = target;
}

// ---------  Area ---------------- //

void area_draw(UIItem* This, UIItem* project_to) {

  RGBA_32 color2 = 0xff050505;
  short thick = 3;

  if (This->state == UIIstate::LEAVED || This->state == UIIstate::NONE) {
    color2 = 0xff101010;
    thick = 2;
  }

  Rect<SCR_UINT> rect(This->rect);
  project_to->buff->DrawBounds(rect, color2, thick);
}

void ui_add_area(UIItem* Area) {
  Area->ownbuff = false;
  Area->DrawBody = area_draw;
}

// ------------------ UI Root --------------------------------- //

void UIdraw(UIItem* This, UIItem* project_to) {
  RGBA_32 color = 0xff1d1d21;
  This->buff->clear(&color);
}

void ui_add_root(UIItem * UIroot) {
  UIroot->DrawBody = UIdraw;
  UIroot->ownbuff = true;;
  UIroot->buff->resize(UIroot->rect.size.x, UIroot->rect.size.y);
}

// ---------------------- UI compiling -------------------------  //

void dimentions_db_to_rect(Rect<float>& rect, DataBlock* db) {
  DataBlock* rectdb = db->find("Dimentions");
  DataBlock* size = rectdb->find("Size");
  DataBlock* pos = rectdb->find("Pos");
  rect.size = vec2<float>((float)size->list[0]->integer, (float)size->list[1]->integer);
  rect.pos = vec2<float>((float)pos->list[0]->integer, (float)pos->list[1]->integer);
}

struct PreCompUII {
  PreCompUII(UIItem* item, Str* parent) {
    this->item = item;
    this->parent = parent;
  }
  UIItem* item;
  Str* parent;
};

void UIItem::Compile(List<Operator>* ops, DataBlock* db, Window* prnt) {

  List<PreCompUII> pcuii;
  UIItem* self = nullptr; 

  FOREACH(&db->list, DataBlock, inode) {

    DataBlock* UIdb = inode->Data;
    UIItem* uiitem = NEW_DBG(UIItem) UIItem();

    uiitem->hrchy.id = UIdb->find("Name")->string; 
    dimentions_db_to_rect(uiitem->rect, UIdb);

    Str* uiitype = &UIdb->find("Type")->string; 
    if (*uiitype == "canvas") {
      ui_add_root(uiitem);
    }

    pcuii.add(NEW_DBG(PreCompUII) PreCompUII(uiitem, &UIdb->find("Parent")->string)); 
  }

  FOREACH(&pcuii, PreCompUII, inode) {
    if (!(*inode->Data->parent == "None")) {
      FOREACH(&pcuii, PreCompUII, jnode) {
        if (*jnode->Data->parent == inode->Data->item->hrchy.id) {
          jnode->Data->item->hrchy.join(inode->Data->item);
          break;
        }
      }
    }
  }


  pcuii.del();
}
