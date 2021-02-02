
#include "public/UILayout.h"
#include "Object.h"
#include "public/KeyMap.h"
#include "public/Seance.h"



UIItem::UIItem(vec2<SCR_UINT>* size) {

  flag = 0;
  rigid.assign(false, false);
  state = UIstate::NONE;
  crnr = UIAttachCorner::BOTTOMLEFT;
  ProcBody = nullptr;
  DrawBody = nullptr;

  if (this->ownbuff = (bool)size) {
    buff = NEW_DBG(FBuff<RGBA_32>) FBuff<RGBA_32>();
    buff->resize(size->x, size->y);
  }
}

UIItem::~UIItem() {
  hierarchy.childs.del();
  if (buff) {
    DELETE_DBG(FBuff<RGBA_32>, buff);
  }
  // if (CustomData)
  // DELETE_DBG() CustomData;
}

void UIItem::ProcEvent(List<OpThread>* op_threads, struct UserInputs* user_inputs, vec2<SCR_UINT>& cursor, Seance* C) {

  if (hide)
    return;

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

  if (hide)
    return;

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

void UIItem::Resize(vec2<float>& rescale) {

  if (!hierarchy.parent) {
    update_neighbors(true);
  }

  prev_rect = rect;
  Rect<float>& prnt_rec = hierarchy.parent ? hierarchy.parent->rect : rect;

  for (char dir = 0; dir <= 1; dir++) {

    if (!rigid[dir]) {
      UIItem* bounds[2] = {OFFSET(wrap.rig, dir), OFFSET(wrap.rig, dir + 2)};

      for (char axes = 0; axes <= 1; axes++) {
        while (bounds[axes]) {
          if (bounds[axes]->rigid[dir]) {
            break;
          }
          bounds[axes] = OFFSET(bounds[axes]->wrap.rig, dir + (char)(2 * axes));
        }
      }

      if ((bounds[0] && bounds[0]->rigid[dir] && !bounds[0]->hide) || (bounds[1] && bounds[1]->rigid[dir]) && !bounds[1]->hide) {

        float pls_dir_pos[2];
        float mns_dir_pos[2];

        if (bounds[0] && bounds[0]->rigid[dir] && !bounds[0]->hide) {
          pls_dir_pos[0] = bounds[0]->rect.pos[dir] + (bounds[0]->rect.size[dir] * (!dir));
          pls_dir_pos[1] = bounds[0]->prev_rect.pos[dir] + (bounds[0]->prev_rect.size[dir] * (!dir));
        } else {
          pls_dir_pos[0] = pls_dir_pos[1] = prnt_rec.size[dir] * (!dir);
        }

        if (bounds[1] && bounds[1]->rigid[dir] && !bounds[1]->hide) {
          mns_dir_pos[0] = bounds[1]->rect.pos[dir] + (bounds[1]->rect.size[dir] * (!dir));
          mns_dir_pos[1] = bounds[1]->prev_rect.pos[dir] + (bounds[1]->prev_rect.size[dir] * (!dir));
        } else {
          mns_dir_pos[0] = mns_dir_pos[1] = prnt_rec.size[dir] * (!dir);
        }

        float pls_fac = (pls_dir_pos[0] - mns_dir_pos[1]) / (pls_dir_pos[1] - mns_dir_pos[1]);
        float mns_fac = (pls_dir_pos[1] - mns_dir_pos[0]) / (pls_dir_pos[1] - mns_dir_pos[1]);

        rect.pos[dir] -= mns_dir_pos[1];
        float pls_width = (rect.size[dir] + rect.pos[dir]) * pls_fac;
        rect.pos[dir] *= pls_fac;
        rect.size[dir] = pls_width - rect.pos[dir];
        rect.pos[dir] += mns_dir_pos[1];

        float d1 = (pls_dir_pos[0] - (rect.pos[dir] + rect.size[dir])) * mns_fac;
        float d2 = (pls_dir_pos[0] - rect.pos[dir]) * mns_fac;
        float pos = pls_dir_pos[0] - d2;
        rect.size[dir] = pls_dir_pos[0] - rect.pos[dir] - d1;
        rect.pos[dir] = pos;

      } else {
        float width = (rect.size[dir] + rect.pos[dir]) * rescale[dir];
        rect.pos[dir] *= rescale[dir];
        rect.size[dir] = width - rect.pos[dir];
      }
    }
  }

  if (rigid.x || rigid.y) {
    if (crnr == UIAttachCorner::BOTTOMRIGGHT || crnr == UIAttachCorner::TOPRIGHT) {
      float dx = prnt_rec.size.x - hierarchy.parent->prev_rect.size.x;
      rect.pos.x += dx;
    }

    if (crnr == UIAttachCorner::TOPLEFT || crnr == UIAttachCorner::TOPRIGHT) {
      float dy = prnt_rec.size.y - hierarchy.parent->prev_rect.size.y;
      rect.pos.y += dy;
    }
  }

  if (hierarchy.parent) {
    if (rigid.x || rigid.y) {
      hide = false;
      if (rect.pos.x + rect.size.x > prnt_rec.size.x || rect.pos.y + rect.size.y > prnt_rec.size.y || rect.pos.x < 0 || rect.pos.y < 0) {
        hide = true;
        if (buff) {
          buff->free();
        }
      }
    }
  }

  if (ownbuff && !hide) {
    buff->resize((SCR_UINT)rect.size.x, (SCR_UINT)rect.size.y);
  }

  vec2<float> chld_rscl(rect.size.x / prev_rect.size.x, rect.size.y / prev_rect.size.y);

  FOREACH_NODE(UIItem, (&hierarchy.childs), child_node) {
    if (child_node->Data->rigid.y || child_node->Data->rigid.x) {
      child_node->Data->Resize(chld_rscl);
    }
  }

  FOREACH_NODE(UIItem, (&hierarchy.childs), child_node) {
    if (!(child_node->Data->rigid.x || child_node->Data->rigid.y)) {
      child_node->Data->Resize(chld_rscl);
    }
  }

  redraw = true;
}

void UIItem::update_neighbors(bool recursive) {

  FOREACH(&hierarchy.childs, UIItem, ui_node) {
    ui_node->Data->wrap.bot = nullptr;
    ui_node->Data->wrap.rig = nullptr;
    ui_node->Data->wrap.lef = nullptr;
    ui_node->Data->wrap.top = nullptr;
  }

  float dist_t = FLT_MAX;
  float dist_b = FLT_MAX;
  float dist_l = FLT_MAX;
  float dist_r = FLT_MAX;

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

UIItem* ui_add_button(UIItem* parent, vec2<SCR_UINT> pos, List<Operator>* operators, Str* op_idname, vec2<bool> rs_type, UIAttachCorner crnr) {

  UIItem* button = NEW_DBG(UIItem) UIItem(nullptr);

  button->hierarchy.join(parent);
  button->rigid = rs_type;
  button->ownbuff = false;
  button->DrawBody = button_draw;
  button->ProcBody = button_proc;
  button->crnr = crnr;
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

UIItem* ui_add_region(UIItem* parent, Rect<SCR_UINT> rect, List<Operator>* operators, vec2<bool> rs_type, UIAttachCorner crnr) {


  UIItem* region = NEW_DBG(UIItem) UIItem(&rect.size);

  region->hierarchy.join(parent);

  region->ownbuff = true;
  region->DrawBody = region_draw;
  region->ProcBody = region_proc;
  region->crnr = crnr;
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

UIItem* ui_add_area(UIItem* parent, Rect<SCR_UINT> rect, Str name, vec2<bool> rs_type, UIAttachCorner crnr) {

  UIItem* Area = NEW_DBG(UIItem) UIItem(nullptr);

  Area->hierarchy.join(parent);

  Area->ownbuff = false;
  Area->DrawBody = area_draw;
  Area->ProcBody = area_proc;
  Area->idname = name;
  Area->rigid = rs_type;
  Area->crnr = crnr;
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
  UIroot->crnr = UIAttachCorner::BOTTOMLEFT;
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

  UIItem* Area = ui_add_area(UIroot, Rect<SCR_UINT>(100, 100, 300, 300), "View3d", vec2<bool>(0, 0), UIAttachCorner::TOPRIGHT);

  UIItem* Region = ui_add_region(Area, Rect<SCR_UINT>(5, 5, 290, 290), operators, vec2<bool>(0, 0), UIAttachCorner::BOTTOMLEFT);

  UIItem* Button = ui_add_button(Area, vec2<SCR_UINT>(200, 200), operators, &Str("Add Plane"), vec2<bool>(1, 1), UIAttachCorner::TOPRIGHT);

  short width = 25;
  short border = 10;
  Rect<SCR_UINT> rect = Rect<SCR_UINT>(border, (SCR_UINT)UIroot->rect.size.y - width - border, (SCR_UINT)UIroot->rect.size.x - border * 2, width);
  UIItem* Area2 = ui_add_area(UIroot, rect, "topbar", vec2<bool>(0, 1), UIAttachCorner::TOPLEFT);

  ui_add_button(Area2, vec2<SCR_UINT>(3, 3), operators, &Str("Toggle Console"), vec2<bool>(1, 1), UIAttachCorner::BOTTOMLEFT);
  ui_add_button(Area2, vec2<SCR_UINT>(3 + 40 * 1, 3), operators, &Str("End Seance"), vec2<bool>(1, 1), UIAttachCorner::BOTTOMLEFT);
  ui_add_button(Area2, vec2<SCR_UINT>(3 + 40 * 2, 3), operators, &Str("Log Heap"), vec2<bool>(1, 1), UIAttachCorner::BOTTOMLEFT);
  return UIroot;
}
