
#include "Data/Seance.h"

#include "Operator/Operator.h"

#include "Platform/SysHandler.h"

#include "UI/UInputs.h"
#include "UI/UInputsMap.h"
#include "UI/UInterface.h"

#include "IO/Parser.h"
#include "Object/Object.h"

Seance::Seance() {
  ui.sysh = NEW(SysHandler)();
  ui.kmap = NEW(KeyMap)();
  ui.UIroot = nullptr;
}

Seance::~Seance() {}

void Seance::OnWrite(/*file path*/) {}

void Seance::OnRead(Str* path, UIItem* (*UIIFromStr)(Str* id, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb)) {

  Str ui_path;
  ui_path = *path;
  ui_path += Str("UInterface\\Default.yaml");
  ui.UIroot = UICompile(&ops, Read_Yaml(&ui_path), UIIFromStr);

  Str km_path;
  km_path = *path;
  km_path += Str("UInputsMap\\Default.yaml");

  ui.kmap->Compile(Read_Yaml(&km_path), &ops, ui.UIroot);

  // Set icon
  Str icon_path;
  icon_path = *path;
  icon_path += Str("icon.ico");
  ui.sysh->SetIcon(icon_path);

}

void UserInterface::Input(Seance& C) {

  if (!sysh->Active()) {
    return;
  }

  sysh->Inputs(*kmap->uinputs);

  if (kmap->uinputs->IsEvent) {
    kmap->evaluate(&C.threads);
    if (UIroot) {
      UIroot->ProcEvent(&C, kmap->uinputs->Cursor);
    }
  }
}

UserInterface::~UserInterface() {
  DEL(UIItem, UIroot);
  DEL(SysHandler, sysh);
  DEL(KeyMap, kmap);
}

void UserInterface::Output() {

  if (kmap->uinputs->IsEvent) {
    if (UIroot) {
      UIroot->Draw(nullptr);
    }
    sysh->Output(UIroot);
  }
}