
#include "public/Window.h"

#include "public/Operator.h"
#include "public/Win32API.h"

void Window::Draw() {
  FOREACH_NODE(UIItem, (&areas), area_node) {
    /*
    Editor* edt = area_node->Data->editor;

    FBuff* fbuff = SysH->getFBuff();

    area_node->Data->rect.v0->assign(0, 0);
    area_node->Data->rect.v1->assign(0, fbuff->height);
    area_node->Data->rect.v2->assign(fbuff->width, fbuff->height);
    area_node->Data->rect.v3->assign(fbuff->width, 0);

    // edt->Draw(scene, &area_node->Data->rect, fbuff);
    */
  }
}

Window::Window(std::string* keymap_path, List<Operator>* operators) {
  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
  assert(SUCCEEDED(CoInitialize(NULL)));
  SystemHandler* SysHdl = new SystemHandler(800, 800);
  assert(SUCCEEDED(SysHdl->Initialize()));
  this->SysH = SysHdl;

  compiled_key_map.Compile(operators, &user_inputs, keymap_path);
}

Window::~Window() { CoUninitialize(); }

void Window::OnWrite() {}

void Window::OnRead() {}

void Window::ProcessEvents(List<OpThread>* op_threads) {
  SysH->getUserInputs(&user_inputs);
  compiled_key_map.ProcEvents(op_threads);
}

void Window::SendBuffToSystem() { SysH->SysOutput(); }
