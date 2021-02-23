
#include "UI/UInterface.h"
#include "Data/Seance.h"
#include "IO/Parser.h"
#include "Object/Object.h"
#include "UI/UInputsMap.h"

UIItem::UIItem(DataBlock* UIdb) {
  flag = 0;
  state = UIIstate::NONE;
  buff = nullptr;

  hrchy.id = UIdb->find("Name")->string;

  DataBlock* size = UIdb->find("Size");
  rect.size = vec2<float>((float)size->list[0].integer, (float)size->list[1].integer);

  DataBlock* pos = UIdb->find("Pos");
  rect.pos = vec2<float>((float)pos->list[0].integer, (float)pos->list[1].integer);

  DataBlock* min = UIdb->find("MinSize");
  minsize = vec2<float>((float)min->list[0].integer, (float)min->list[1].integer);

  DataBlock* rigiddb = UIdb->find("Rigid");
  rigid = vec2<bool>(rigiddb->list[0].boolean, rigiddb->list[1].boolean);
}

UIItem::~UIItem() {
  if (buff) {
    DEL(BitMap<RGBA_32>, buff);
  }
}

UIIstate UIItem::State(vec2<SCR_INT>& cursor) {
  if (rect.inside((float)cursor.x, (float)cursor.y)) {
    if (state == UIIstate::NONE) {
      return UIIstate::ENTERED;
    }
    return UIIstate::INSIDE;
  } else {
    if (state == UIIstate::INSIDE) {
      return UIIstate::LEAVED;
    }
    return UIIstate::NONE;
  }
}

void UIItem::ProcEvent(Seance* C, vec2<SCR_INT>& cursor) {

  IF(hide, return );

  UIIstate newState = State(cursor);

  if (state != newState) {
    redraw = true;
    state = newState;
  }

  if (state == UIIstate::INSIDE) {
    ProcBody(this, C, cursor);
    redraw = true;
  }

  if (redraw) {
    vec2<SCR_INT> pos = vec2<SCR_INT>((SCR_INT)rect.pos.x, (SCR_INT)rect.pos.y);
    FOREACH((&hrchy.childs), UIItem, child_node) { child_node->ProcEvent(C, (cursor - pos)); }
  }
}

void UIItem::Draw(UIItem* project_to) {

  IF(hide, return );

  if (redraw || !ownbuff) {

    UIItem* draw_to = project_to;

    if (ownbuff) {
      draw_to = this;
    }

    IF(draw_to, DrawBody(this, draw_to));

    FOREACH((&hrchy.childs), UIItem, child) { child->Draw(draw_to); }
  }

  if (ownbuff && project_to) {

    vec2<float> wrldpos;
    PosInParent(project_to, wrldpos);

    project_to->buff->Project(buff, vec2<SCR_INT>(wrldpos.x, wrldpos.y));
  }

  redraw = false;
}

void UIItem::Resize(Rect<float>& newrect) {

  update_neighbors(true);
  save_config();

  for (char i = 0; i < 2; i++) {

    rect.pos[i] = newrect.pos[i];
    rect.size[i] = newrect.size[i];

    if (hrchy.prnt && !hrchy.prnt->valid(i)) {
      rect.pos[i] = prev_rect.pos[i];
      rect.size[i] = prev_rect.size[i];
      continue;
    }

    bool root = true;
    clear_flags();

    /*
    if (!resize_dir(newrect.size[i] / prev_rect.size[i], i, root)) {
      resize_discard(i);
    }
    */
  }

  vec2<float> del;
  del = newrect.pos - prev_rect.pos;
  FOREACH(&hrchy.childs, UIItem, node) { node->move(node->rect.pos - del); }

  update_buff(true);
}

bool UIItem::valid(bool dir) {

  FOREACH((&hrchy.childs), UIItem, mainnode) {

    if (!infinite_) {
      if (!mainnode->rect.enclosed_in(rect, true)) {
        return false;
      }
    }

    if (mainnode->minsize[dir] >= mainnode->rect.size[dir]) {
      return false;
    }

    FOREACH((&hrchy.childs), UIItem, comparenode) {
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

bool UIItem::resize_dir(float rescale, bool dir, bool& root) {

  if (!root) {
    ResizeBody(rect, dir);
  } else {
    root = false;
  }

  if (rescale > 1) {
    hrchy.childs.Invert();
  }

  // repead recursively
  float chld_rscl = rect.size[dir] / prev_rect.size[dir];

  FOREACH((&hrchy.childs), UIItem, child_node) {
    if (child_node->rigid[dir]) {
      if (!child_node->resize_dir(chld_rscl, dir, root)) {
        return false;
      }
    }
  }

  FOREACH((&hrchy.childs), UIItem, child_node) {
    if (!(child_node->rigid[dir])) {
      if (!child_node->resize_dir(chld_rscl, dir, root)) {
        return false;
      }
    }
  }

  if (!valid(dir)) {
    return false;
  }

  redraw = true;
  return true;
}

void UIItem::resize_discard(bool dir) {

  rect.size[dir] = prev_rect.size[dir];
  rect.pos[dir] = prev_rect.pos[dir];

  FOREACH((&hrchy.childs), UIItem, child_node) { child_node->resize_discard(dir); }
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
    if (item = node->find(string)) {
      return item;
    }
  }

  return nullptr;
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

void UIItem::update_buff(bool recursive) {
  if (ownbuff && !hide) {
    buff->resize((SCR_INT)rect.size.x, (SCR_INT)rect.size.y);
  }
  FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->update_buff(recursive); }
}

void UIItem::clear_flags() {
  flag = 0;
  FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->clear_flags(); }
}

void UIItem::save_config() {
  prev_rect = rect;
  FOREACH(&hrchy.childs, UIItem, ui_node) { ui_node->save_config(); }
}

UIItem* UIItem::active_lower() {

  UIItem* active = nullptr;

  if (state != UIIstate::INSIDE) {
    return active;
  }

  FOREACH(&hrchy.childs, UIItem, node) {
    if (active = node->active_lower()) {
      return active;
    }
  }

  return this;
}

void UIItem::move(vec2<float> pos) {
  prev_rect = rect;
  for (char i = 0; i < 2; i++) {
    rect.pos[i] = pos[i];
    if (hrchy.prnt && !hrchy.prnt->valid(i)) {
      rect.pos[i] = prev_rect.pos[i];
    }
  }
}

void UIItem::PosInParent(UIItem* inframe, vec2<float>& out) {

  if (hrchy.prnt) {

    if (inframe && inframe == this) {
      return;
    }

    hrchy.prnt->PosInParent(inframe, out);
    out += rect.pos;
    return;
  }

  out.assign(0, 0);
}


// ---------------------- UI compiling -------------------------  //

struct PreCompUII {
  PreCompUII(UIItem* item, Str* parent) {
    this->item = item;
    this->parent = parent;
  }
  UIItem* item;
  Str* parent;
};

UIItem* UICompile(Operators* ops, DataBlock* db, UIItem* (*UIIFromStr)(Str* id, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb)) {

  UIItem* root = nullptr;
  List<PreCompUII> pcuii;
  DataBlock* uilistdb = db->find("UIItems");

  FOREACH(&uilistdb->list, DataBlock, UIdb) {

    UIItem* newuii = nullptr;
    Str* parent = &UIdb->find("Parent")->string;

    DataBlock* templatedb = UIdb->find("Template");
    DataBlock* usingdb = templatedb->find("Using");
    DataBlock* withdb = templatedb->find("With");

    newuii = UIIFromStr(&usingdb->string, ops, withdb, UIdb.Data());
    if (newuii) {
      pcuii.PushBack(NEW(PreCompUII)(newuii, parent));
    }
  }

  FOREACH(&pcuii, PreCompUII, inode) {
    if (!(*inode->parent == "__NONE__")) {
      FOREACH(&pcuii, PreCompUII, jnode) {
        if (*inode->parent == jnode->item->hrchy.id) {
          jnode->item->hrchy.childs.PushBack(inode->item);
          inode->item->hrchy.prnt = jnode->item;
          break;
        }
      }
    } else {
      root = inode->item;
    }
  }

  return root;
}
