
#include "public/Window.h"
#include "public/Win32API.h"
#include "public/Operator.h"

void Window::Draw() {

  FOREACH_NODE(ScrArea, (&scrAreas), area_node) {
    Editor* edt = area_node->Data->editor;

    FBuff* fbuff = SysH->getFBuff();

    area_node->Data->rect.v0->assign(0, 0);
    area_node->Data->rect.v1->assign(0, fbuff->height);
    area_node->Data->rect.v2->assign(fbuff->width, fbuff->height);
    area_node->Data->rect.v3->assign(fbuff->width, 0);

    //edt->Draw(scene, &area_node->Data->rect, fbuff);
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
    char* op_event = op_key_map.second->IsOPEvent(&user_inputs);
    if (op_event) {
      exec_queue->add(new ExecComand(op_key_map.second->op_ptr, op_event));
    }
  }
}

void Window::SendBuffToSystem() { SysH->SysOutput(); }
