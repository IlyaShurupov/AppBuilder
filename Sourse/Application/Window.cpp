
#include "public/Window.h"

#include "public/Operator.h"
#include "public/Win32API.h"

void Window::Draw() {
  FOREACH_NODE(ScrArea, (&scrAreas), area_node) {
    Editor* edt = area_node->Data->editor;

    FBuff* fbuff = SysH->getFBuff();

    area_node->Data->rect.v0->assign(0, 0);
    area_node->Data->rect.v1->assign(0, fbuff->height);
    area_node->Data->rect.v2->assign(fbuff->width, fbuff->height);
    area_node->Data->rect.v3->assign(fbuff->width, 0);

    // edt->Draw(scene, &area_node->Data->rect, fbuff);
  }
}

Window::Window() {
  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
  assert(SUCCEEDED(CoInitialize(NULL)));
  SystemHandler* SysHdl = new SystemHandler(800, 800);
  assert(SUCCEEDED(SysHdl->Initialize()));
  this->SysH = SysHdl;
}

Window::~Window() { CoUninitialize(); }

void Window::OnWrite() {}

void Window::OnRead() {}

void Window::ProcessEvents(KeyMap* key_map, List<ExecComand>* exec_queue) {
  SysH->getUserInputs(&user_inputs);

  for (auto op_key_map : key_map->map) {
    OperatorBindings* bindings = op_key_map.second;
    Operator* op = bindings->op_ptr;

    if (op->state == OpState::RUNNING_MODAL) {
      ModalEvent* mev = bindings->IsModalEvent(&user_inputs);
      if (mev) {
        exec_queue->add(new ExecComand(op, OpEventState::MODAL_EVENT, mev));
      }
    } else {
      if (bindings->IsInvoked(&user_inputs)) {
        exec_queue->add(new ExecComand(op, OpEventState::INVOKE, nullptr));
      }
    }
  }
}

void Window::SendBuffToSystem() { SysH->SysOutput(); }
