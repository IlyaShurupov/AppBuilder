
#include "public/UILayout.h"
#include "public/Seance.h"


// ---------  Add Button ---------------- //

typedef struct Button {
  vec2<SCR_UINT> pos;
  std::string name;
  std::string op_idname;
  Operator* op = nullptr;
} Button;

void button_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs) {
  if (/*active*/ true) {

  }
}

void button_draw(UIItem* This) {

}

UIItem* ui_add_region(UIItem* parent, vec2<SCR_UINT> pos, std::string name, std::string op_idname) {
  UIItem* button = DBG_NEW UIItem(UIType::BUTTON);

  button->Draw = region_draw;
  button->ProcEvent = region_proc;

  Button* bd = DBG_NEW Button;
  button->CustomData = (void*)bd;

  bd->name = name;
  bd->pos = pos;
  bd->op_idname = op_idname;

  button->hierarchy.join(parent);
  return button;
}

// --------- Add Region ---------------- //

typedef struct RegionData {
  // location & size in the parent frame of reference
  Rect<SCR_UINT> rect;
  std::string name;
} RegionData;

void region_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs) {

  if (/*active*/ true) {
    FOREACH_NODE(UIItem, (&This->hierarchy.childs), child_node) {
      child_node->Data->ProcEvent(child_node->Data, op_threads, user_inputs);
    }
  }
}

void region_draw(UIItem* This) {
  FOREACH_NODE(UIItem, (&This->hierarchy.childs), child_node) {
    child_node->Data->Draw(child_node->Data);
  }
}

UIItem* ui_add_region(UIItem* parent, Rect<SCR_UINT> rect, std::string name) {
  UIItem* region = DBG_NEW UIItem(UIType::REGION);

  region->Draw = region_draw;
  region->ProcEvent = region_proc;

  RegionData* rd = DBG_NEW RegionData;
  region->CustomData = (void*)rd;

  rd->rect = rect;
  rd->name = name;

  region->hierarchy.join(parent);
  return region;
}


// ---------  Add Area ---------------- //

typedef struct AreaData {
  // location & size in the parent frame of reference
  Rect<SCR_UINT> rect;
  std::string name;
} AreaData;

void area_proc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs) {

  if (/*active*/ true) {
    FOREACH_NODE(UIItem, (&This->hierarchy.childs), child_node) {
      child_node->Data->ProcEvent(child_node->Data, op_threads, user_inputs);
    }
  }
}

void area_draw(UIItem* This) {
  FOREACH_NODE(UIItem, (&This->hierarchy.childs), child_node) {
    child_node->Data->Draw(child_node->Data);
  }
}

UIItem* ui_add_area(UIItem* parent, Rect<SCR_UINT> rect, std::string name) {
  UIItem* Area = DBG_NEW UIItem(UIType::AREA);

  Area->Draw = area_draw;
  Area->ProcEvent = area_proc;

  AreaData* ad = DBG_NEW AreaData;
  Area->CustomData = (void*)ad;

  ad->rect = rect;
  ad->name = name;

  Area->hierarchy.join(parent);
  return Area;
}

// ------------------ Root --------------------------------- //
void Uiproc(UIItem* This, List<OpThread>* op_threads, struct UserInputs* user_inputs) {
  FOREACH_NODE(UIItem, (&This->hierarchy.childs), child_node) {
    child_node->Data->ProcEvent(child_node->Data, op_threads, user_inputs);
  }
}

void UIdraw(UIItem* This) {
  FOREACH_NODE(UIItem, (&This->hierarchy.childs), child_node) {
    child_node->Data->Draw(child_node->Data);
  }
}

// ---------------------- UI compiling -------------------------  //

UIItem* UI_compile(List<Operator>* operators, std::string *ui_path) {

  UIItem* UIroot =  DBG_NEW UIItem(UIType::ROOT);
  UIroot->ProcEvent = Uiproc;
  UIroot->Draw = UIdraw;

  //UIItem* Area = add_ui_area_3dview(UIroot);

  //UIItem* Region = DBG_NEW UIItem(UIType::REGION);
  //Area->hierarchy.add(Region);

  return UIroot;
}
