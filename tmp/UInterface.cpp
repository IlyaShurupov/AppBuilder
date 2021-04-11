
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

  DataBlock* rigiddb = UIdb->find("Rigid");
  rigid = vec2<bool>(rigiddb->list[0].boolean, rigiddb->list[1].boolean);
}

UIItem::~UIItem() {
  if (buff) {
    DEL(DevBuffer, buff);
  }
}

void UIItem::ProcEvent(Seance* C, vec2<SCR_INT> cursor) {

  IF(hide, return );

          UIIstate newState;

        if (rect.inside((float)cursor.x, (float)cursor.y)) {
            if (state == UIIstate::NONE) {
            newState = UIIstate::ENTERED;
            }
            newState = UIIstate::INSIDE;
        } else {
            if (state == UIIstate::INSIDE) {
            newState = UIIstate::LEAVED;
            }
            newState = UIIstate::NONE;
        }

        if (state != newState) {
            redraw = true;
            state = newState;
        }

        if (state == UIIstate::INSIDE) {
            OnUpdateBody(requests, inputs);
            redraw = true;
        }

        if (redraw) {
            vec2<SCR_INT> pos = vec2<SCR_INT>(rect.pos.x, rect.pos.y);

            FOREACH((&hrchy.childs), UIItem, child_node) { child_node->ProcEvent(C, ); }
                ((Guii*)guii.Data())->OnUpdate(requests, inputs, (cursor - pos));
            }
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

  save_config();

  for (char i = 0; i < 2; i++) {

    rect.pos[i] = newrect.pos[i];
    rect.size[i] = newrect.size[i];

    if (!hrchy.prnt->Valid(i)) {
      rect.pos[i] = prev_rect.pos[i];
      rect.size[i] = prev_rect.size[i];
      continue;
    }

    if (recursive_trunsform) {
      bool root = true;
      if (!resize_dir(newrect.size[i] / prev_rect.size[i], i, root)) {
        resize_discard(i);
      }
    }
  }

  if (!recursive_trunsform) {
    vec2<float> del;
    del = rect.pos - prev_rect.pos;
    FOREACH(&hrchy.childs, UIItem, node) {
      node->rect.pos -= del;
    }
  }

  update_buff(true);
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

  if (!Valid(dir)) {
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

void UIItem::MoveChilds(vec2<float>& delta) {
  
  save_config();

  FOREACH(&hrchy.childs, UIItem, chld) {
    chld->rect.pos += delta;
  }

  for (char i = 0; i < 2; i++) {
    if (!Valid(i)) {
      resize_discard(i);
    }
  }
}

void UIItem::move(vec2<float> pos) {
  prev_rect = rect;

  for (char i = 0; i < 2; i++) {
    rect.pos[i] = pos[i];
    if (!hrchy.prnt->Valid(i)) {
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
    if (!(*inode->parent == Str("__NONE__"))) {
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
