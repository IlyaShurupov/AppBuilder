
#include "public/Window.h"
#include "public/Win32API.h"


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

void Window::ProcessEvents() {
  UserInputs* user_inputs = SysH->getUserInputs();
}

void Window::SendBuffToSystem() { SysH->SysOutput(); }
