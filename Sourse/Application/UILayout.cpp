
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
  hierarchy.childs.del();
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
    FOREACH_NODE(UIItem, (&hierarchy.childs), child_node) {
      vec2<SCR_UINT> pos = vec2<SCR_UINT>((SCR_UINT)rect.pos.x, (SCR_UINT)rect.pos.y);
      child_node->Data->ProcEvent(op_threads, user_inputs, (cursor - pos), C);
    }
  }
}

void UIItem::Draw(UIItem* project_to) {

  IF(hide, return );

  IF(DrawBody, DrawBody(this, project_to));

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

void UIItem::Resize(float rescale, bool dir) {

  if (!hierarchy.parent) {
    prev_rect.size[dir] = rect.size[dir];
    rect.size[dir] *= rescale;
    goto SKIP;
  }

  Rect<float>* prnt_p_rec = &hierarchy.parent->prev_rect;
  Rect<float>* prnt_rec = &hierarchy.parent->rect;
  float bounds[4];
  UIItem* UIrigid;
  float* bnds;

  prev_rect = rect;

  if (!rigid[dir]) {

    for (char offset = 0; offset <= 2; offset += 2) {

      UIrigid = OFFSET(wrap.rig, offset + dir);
      bnds = bounds + offset;

      while (UIrigid) {
        DOIF(break, UIrigid->rigid[dir]);
        UIrigid = OFFSET(UIrigid->wrap.rig, dir + offset);
      }

      if (UIrigid && UIrigid->rigid[dir] && !UIrigid->hide) {

        bnds[0] = UIrigid->rect.pos[dir] + (UIrigid->rect.size[dir] * (bool)offset);
        bnds[1] = UIrigid->prev_rect.pos[dir] + (UIrigid->prev_rect.size[dir] * (bool)offset);

      } else {
        bnds[0] = prnt_rec->size[dir] * (bool)offset;
        bnds[1] = prnt_p_rec->size[dir] * (bool)offset;
      }
    }

    float* pls_dir = bounds + 2;
    float* mns_dir = bounds;

    float pls_fac = (pls_dir[0] - mns_dir[1]) / (pls_dir[1] - mns_dir[1]);
    float mns_fac = (pls_dir[1] - mns_dir[0]) / (pls_dir[1] - mns_dir[1]);

    rect.pos[dir] -= mns_dir[1];
    float pls_width = (rect.size[dir] + rect.pos[dir]) * pls_fac;
    rect.pos[dir] *= pls_fac;
    rect.size[dir] = pls_width - rect.pos[dir];
    rect.pos[dir] += mns_dir[1];

    float d1 = (pls_dir[0] - (rect.pos[dir] + rect.size[dir])) * mns_fac;
    float d2 = (pls_dir[0] - rect.pos[dir]) * mns_fac;
    float pos = pls_dir[0] - d2;
    rect.size[dir] = pls_dir[0] - rect.pos[dir] - d1;
    rect.pos[dir] = pos;

  } else if (inv_pos[dir]) {
    rect.pos[dir] += prnt_rec->size[dir] - hierarchy.parent->prev_rect.size[dir];
  }

  // Hide if overlaped or min size triggered
  if (rigid.x || rigid.y) {

    hide = false;

    if (rect.pos.x + rect.size.x > prnt_rec->size.x || rect.pos.y + rect.size.y > prnt_rec->size.y || rect.pos.x < 0 || rect.pos.y < 0) {

      hide = true;

      if (buff) {
        buff->free();
      }
    }
  }

SKIP:

  // repead recursively
  float chld_rscl = rect.size[dir] / prev_rect.size[dir];

  FOREACH_NODE(UIItem, (&hierarchy.childs), child_node) {
    if (child_node->Data->rigid[dir]) {
      child_node->Data->Resize(chld_rscl, dir);
    }
  }

  FOREACH_NODE(UIItem, (&hierarchy.childs), child_node) {
    if (!(child_node->Data->rigid[dir])) {
      child_node->Data->Resize(chld_rscl, dir);
    }
  }

  redraw = true;
}

void UIItem::update_neighbors(bool recursive) {

  float dist_t = FLT_MAX;
  float dist_b = FLT_MAX;
  float dist_l = FLT_MAX;
  float dist_r = FLT_MAX;

  FOREACH(&hierarchy.childs, UIItem, ui_node) {
    for (char i = 0; i < 4; i++) {
      OFFSET(ui_node->Data->wrap.rig, i) = nullptr;
    }
  }

  FOREACH(&hierarchy.childs, UIItem, cld_node) {
    UIItem& cld = *cld_node->Data;

    if (cld.wrap.top && cld.wrap.bot && cld.wrap.lef && cld.wrap.rig) {
      continue;
    }

    FOREACH(&hierarchy.childs, UIItem, ui_node) {
      Rect<float>& i_rec = ui_node->Data->rect;

      bool intr_y = cld.rect.intersect_y(i_rec);
      bool intr_x = cld.rect.intersect_x(i_rec);

      if (intr_y) {
        if (!cld.wrap.top && cld.rect.above(i_rec) && dist_t > cld.rect.pos.y + cld.rect.size.y - i_rec.pos.y) {

          cld.wrap.top = ui_node->Data;
          ui_node->Data->wrap.bot = &cld;

        } else if (!cld.wrap.bot && cld.rect.bellow(i_rec) && dist_b > cld.rect.pos.y - i_rec.pos.y + i_rec.size.y) {

          cld.wrap.bot = ui_node->Data;
          ui_node->Data->wrap.top = &cld;
        }

      } else if (intr_x) {
        if (!cld.wrap.rig && cld.rect.right(i_rec) && dist_r > cld.rect.pos.x + cld.rect.size.x - i_rec.pos.x) {

          cld.wrap.rig = ui_node->Data;
          ui_node->Data->wrap.lef = &cld;

        } else if (!cld.wrap.lef && cld.rect.left(i_rec) && dist_l > cld.rect.pos.x - i_rec.pos.x + i_rec.size.x) {

          cld.wrap.lef = ui_node->Data;
          ui_node->Data->wrap.rig = &cld;
        }
      }

      if (cld.wrap.top && cld.wrap.bot && cld.wrap.lef && cld.wrap.rig) {
        break;
      }
    }
  }

  if (recursive) {
    FOREACH(&hierarchy.childs, UIItem, ui_node) { ui_node->Data->update_neighbors(recursive); }
  }
}

void UIItem::update_buff(bool recursive) {
  IF((ownbuff && !hide), buff->resize((SCR_UINT)rect.size.x, (SCR_UINT)rect.size.y));
  FOREACH(&hierarchy.childs, UIItem, ui_node) { ui_node->Data->update_buff(recursive); }
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

UIItem* ui_add_button(UIItem* parent, vec2<SCR_UINT> pos, List<Operator>* operators, Str* op_idname, vec2<bool> rs_type, vec2<bool> inv_pos) {

  UIItem* button = NEW_DBG(UIItem) UIItem(nullptr);

  button->hierarchy.join(parent);
  button->rigid = rs_type;
  button->ownbuff = false;
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

UIItem* ui_add_region(UIItem* parent, Rect<SCR_UINT> rect, List<Operator>* operators, vec2<bool> rs_type, vec2<bool> inv_pos) {


  UIItem* region = NEW_DBG(UIItem) UIItem(&rect.size);

  region->hierarchy.join(parent);

  region->ownbuff = true;
  region->DrawBody = region_draw;
  region->ProcBody = region_proc;
  region->inv_pos = inv_pos;
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

UIItem* ui_add_area(UIItem* parent, Rect<SCR_UINT> rect, Str name, vec2<bool> rs_type, vec2<bool> inv_pos) {

  UIItem* Area = NEW_DBG(UIItem) UIItem(nullptr);

  Area->hierarchy.join(parent);

  Area->ownbuff = false;
  Area->DrawBody = area_draw;
  Area->ProcBody = area_proc;
  Area->idname = name;
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

UIItem* ui_add_root(Rect<SCR_UINT> rect) {

  UIItem* UIroot = NEW_DBG(UIItem) UIItem(&rect.size);

  UIroot->ProcBody = Uiproc;
  UIroot->DrawBody = UIdraw;
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

UIItem* UI_compile(List<Operator>* operators, Str* ui_path, Window* parent) {

  UIItem* UIroot = ui_add_root(Rect<SCR_UINT>(550, 200, 900, 600));

  UIItem* Area = ui_add_area(UIroot, Rect<SCR_UINT>(100, 100, 300, 300), "View3d", vec2<bool>(0, 0), vec2<bool>(1, 1));

  UIItem* Region = ui_add_region(Area, Rect<SCR_UINT>(5, 5, 290, 290), operators, vec2<bool>(0, 0), vec2<bool>(0, 0));

  UIItem* Button = ui_add_button(Area, vec2<SCR_UINT>(200, 200), operators, &Str("Add Plane"), vec2<bool>(1, 1), vec2<bool>(1, 1));

  short width = 25;
  short border = 10;
  Rect<SCR_UINT> rect = Rect<SCR_UINT>(border, (SCR_UINT)UIroot->rect.size.y - width - border, (SCR_UINT)UIroot->rect.size.x - border * 2, width);
  UIItem* Area2 = ui_add_area(UIroot, rect, "topbar", vec2<bool>(0, 1), vec2<bool>(0, 1));

  ui_add_button(Area2, vec2<SCR_UINT>(3, 3), operators, &Str("Toggle Console"), vec2<bool>(1, 1), vec2<bool>(0, 0));
  ui_add_button(Area2, vec2<SCR_UINT>(3 + 40 * 1, 3), operators, &Str("End Seance"), vec2<bool>(1, 1), vec2<bool>(0, 0));
  ui_add_button(Area2, vec2<SCR_UINT>(3 + 40 * 2, 3), operators, &Str("Log Heap"), vec2<bool>(1, 1), vec2<bool>(0, 0));

  UIItem* Area3 = ui_add_area(UIroot, Rect<SCR_UINT>(100, 5, 150, 50), "bottom bar", vec2<bool>(0, 1), vec2<bool>(0, 0));
  return UIroot;
}
