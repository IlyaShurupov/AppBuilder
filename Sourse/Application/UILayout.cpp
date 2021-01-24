
#include "public/UILayout.h"
#include "public/Seance.h"


// UI Item Button

// ---------  Region 3DViewport ---------------- //

typedef struct Region3DView {

} Region3DView;

void region_3dview_proc(UIItem* This, struct UserInputs* user_inputs) {

}

void region_3dview_draw(UIItem* This) {

}

static void create_ui_region_3dview() {

}


// ---------  Area 3DView ---------------- //

typedef struct Area3DView {

} Area3DView;

void area_3dview_proc(UIItem* This, struct UserInputs* user_inputs) {

}

void area_3dview_draw(UIItem* This) {

}

static void create_ui_area_3dview() {

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

UIItem* UIroot_Init(List<Operator>* operators) {

  UIItem* UIroot =  DBG_NEW UIItem(UIType::ROOT);
  UIroot->ProcEvent = Uiproc;
  UIroot->Draw = UIdraw;

  UIItem* Area = DBG_NEW UIItem(UIType::AREA);
  UIroot->hierarchy.add(Area);

  UIItem* Region = DBG_NEW UIItem(UIType::REGION);
  Area->hierarchy.add(Region);

  return UIroot;
}
