
#include "public/UILayout.h"
#include "Object.h"
#include "public/KeyMap.h"
#include "public/Seance.h"

UIItem::UIItem(vec2<SCR_UINT>* size) {

  flag = 0;
  rigid.assign(false, false);
  state = UIstate::NONE;
  inv_pos.assign(0, 0);
  ProcBody = nullptr;
  DrawBody = nullptr;

  if (this->ownbuff = (bool)size) {
    buff = NEW_DBG(FBuff<RGBA_32>) FBuff<RGBA_32>(size->x, size->y);
  }
}

UIItem::~UIItem() {
  hrchy.childs.del();
  if (buff) {
    DELETE_DBG(FBuff<RGBA_32>, buff);
  }
}

void UIItem::ProcEvent(List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {

  IF(hide, return );

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

void button_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {
  if (user_inputs->LMB.state == InputState::RELEASED) {
    op_threads->add(NEW_DBG(OpThread) OpThread((Operator*)This->CustomData, OpEventState::EXECUTE, nullptr));
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

UIItem* ui_add_button(UIItem* prnt, vec2<SCR_UINT> pos, vec2<float> minsz, List<Operator>* operators, Str* op_idname, vec2<bool> rs_type, vec2<bool> inv_pos) {

  UIItem* button = NEW_DBG(UIItem) UIItem(nullptr);

  button->hrchy.join(prnt);
  button->rigid = rs_type;
  button->ownbuff = false;
  button->minsize = minsz;
  button->DrawBody = button_draw;
  button->ProcBody = button_proc;
  button->inv_pos = inv_pos;
  button->rect.size.assign(40, 20);
  button->rect.pos.assign((float)pos.x, (float)pos.y);

  // own
  Operator* op_ptr = find_op(operators, op_idname);
  if (!op_ptr) {
    return nullptr;
  }
  button->CustomData = (void*)op_ptr;
  // own

  return button;
}


// --------- Region ---------------- //

typedef struct UIRegionData {
  Operator* op = nullptr;
  Object* RS_ptr = nullptr;
} UIRegionData;

void region_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {

  UIRegionData* rd = (UIRegionData*)This->CustomData;

  if (rd->RS_ptr) {

    op_threads->add(NEW_DBG(OpThread) OpThread(rd->op, OpEventState::EXECUTE, nullptr));

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

void region_draw(UIItem* This, UIItem* project_to) {}

UIItem* ui_add_region(UIItem* prnt, Rect<SCR_UINT> rect, vec2<float> minsz, List<Operator>* operators, vec2<bool> rs_type, vec2<bool> inv_pos) {


  UIItem* region = NEW_DBG(UIItem) UIItem(&rect.size);

  region->hrchy.join(prnt);

  region->ownbuff = true;
  region->DrawBody = region_draw;
  region->ProcBody = region_proc;
  region->inv_pos = inv_pos;
  region->minsize = minsz;
  region->rect.size.assign((float)rect.size.x, (float)rect.size.y);
  region->rect.pos.assign((float)rect.pos.x, (float)rect.pos.y);

  region->rigid = rs_type;

  // own
  Operator* op_ptr = find_op(operators, &Str("Render To Buff"));
  if (!op_ptr) {
    return nullptr;
  }

  UIRegionData* rd = NEW_DBG(UIRegionData) UIRegionData();
  region->CustomData = (void*)rd;
  rd->op = op_ptr;
  // own

  return region;
}


// ---------  Area ---------------- //

void area_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {}

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

UIItem* ui_add_area(UIItem* prnt, Rect<SCR_UINT> rect, vec2<float> minsz, Str name, vec2<bool> rs_type, vec2<bool> inv_pos) {

  UIItem* Area = NEW_DBG(UIItem) UIItem(nullptr);

  Area->hrchy.join(prnt);

  Area->ownbuff = false;
  Area->DrawBody = area_draw;
  Area->ProcBody = area_proc;
  Area->idname = name;
  Area->minsize = minsz;
  Area->rigid = rs_type;
  Area->inv_pos = inv_pos;
  Area->rect.size.assign((float)rect.size.x, (float)rect.size.y);
  Area->rect.pos.assign((float)rect.pos.x, (float)rect.pos.y);

  // own

  return Area;
}

// ------------------ UI Root --------------------------------- //

void Uiproc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& loc_cursor, Seance* C) {}

void UIdraw(UIItem* This, UIItem* project_to) {
  RGBA_32 color = 0xff1d1d21;
  This->buff->clear(&color);
}

UIItem* ui_add_root(Rect<SCR_UINT> rect, vec2<float> minsz) {

  UIItem* UIroot = NEW_DBG(UIItem) UIItem(&rect.size);

  UIroot->ProcBody = Uiproc;
  UIroot->DrawBody = UIdraw;
  UIroot->minsize = minsz;
  UIroot->rigid.assign(false, false);
  UIroot->inv_pos.assign(0, 0);
  UIroot->rect.size.assign((float)rect.size.x, (float)rect.size.y);
  UIroot->rect.pos.assign((float)rect.pos.x, (float)rect.pos.y);

  UIroot->minsize.y = 60;
  UIroot->minsize.x = 100;
  UIroot->ownbuff = true;
  return UIroot;
}

// ---------------------- UI compiling -------------------------  //

UIItem* UI_compile(List<Operator>* operators, Str* ui_path, Window* prnt) {

  UIItem* UIroot = ui_add_root(Rect<SCR_UINT>(550, 200, 900, 600), vec2<float>(30, 30));

  UIItem* Area = ui_add_area(UIroot, Rect<SCR_UINT>(100, 100, 300, 300), vec2<float>(30, 30), "View3d", vec2<bool>(0, 0), vec2<bool>(1, 1));

  UIItem* Region = ui_add_region(Area, Rect<SCR_UINT>(5, 5, 290, 290), vec2<float>(30, 30), operators, vec2<bool>(0, 0), vec2<bool>(0, 0));

  // UIItem* Button = ui_add_button(Region, vec2<SCR_UINT>(200, 200), operators, &Str("Add Plane"), vec2<bool>(1, 1), vec2<bool>(1, 1));

  short width = 25;
  short border = 10;
  Rect<SCR_UINT> rect = Rect<SCR_UINT>(border, (SCR_UINT)UIroot->rect.size.y - width - border, (SCR_UINT)UIroot->rect.size.x - border * 2, width);
  UIItem* Area2 = ui_add_area(UIroot, rect, vec2<float>(30, 10), "topbar", vec2<bool>(0, 1), vec2<bool>(0, 1));

  ui_add_button(Area2, vec2<SCR_UINT>(2, 3), vec2<float>(30, 10), operators, &Str("Toggle Console"), vec2<bool>(1, 1), vec2<bool>(0, 0));
  ui_add_button(Area2, vec2<SCR_UINT>(4 + 40 * 1, 3), vec2<float>(30, 10), operators, &Str("End Seance"), vec2<bool>(1, 1), vec2<bool>(0, 0));
  ui_add_button(Area2, vec2<SCR_UINT>(6 + 40 * 2, 3), vec2<float>(30, 10), operators, &Str("Log Heap"), vec2<bool>(1, 1), vec2<bool>(0, 0));

  UIItem* Area3 = ui_add_area(UIroot, Rect<SCR_UINT>(100, 5, 500, 50), vec2<float>(30, 30), "bottom bar", vec2<bool>(0, 1), vec2<bool>(0, 0));
  UIItem* Area4 = ui_add_area(UIroot, Rect<SCR_UINT>(500, 300, 200, 200), vec2<float>(30, 30), "right", vec2<bool>(1, 0), vec2<bool>(1, 0));
  UIItem* Area5 = ui_add_area(UIroot, Rect<SCR_UINT>(10, 50, 60, 200), vec2<float>(30, 30), "left", vec2<bool>(1, 0), vec2<bool>(0, 0));
  return UIroot;
}
