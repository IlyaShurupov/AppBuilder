
#include "Core/Seance.h"

#include "Core/Operator.h"

#include "Platform/SysHandler.h"

#include "UI/UInputs.h"
#include "UI/UInputsMap.h"
#include "UI/UInterface.h"

#include "Object.h"
#include "IO/Parser.h"

#include "Ops/Ops.h"


Seance::Seance(Str* path) {

  // load project
  if (/*file specified*/ false) {  
    OnRead(/*file path*/);
    return;
  }  

  path->trim(Range(0, path->rfind('\\', Range(0, path->length))));
  *path += Str("Configuration\\");

  Str ui_path;
  ui_path = *path;
  ui_path += Str("UInterface\\Default.yaml");
  ui.UIroot = UICompile(&ops, Read_Yaml(&ui_path));

  Str km_path;
  km_path = *path;
  km_path += Str("UInputsMap\\Default.yaml");

  (ui.kmap = NEW(KeyMap) ())->Compile(Read_Yaml(&km_path), &ops, ui.UIroot);

  ui.sysh = NEW(SysHandler) ();

  // Set icon
  Str icon_path;
  icon_path = *path;
  icon_path += Str("icon.ico");
  ui.sysh->SetIcon(icon_path);

}

Seance::~Seance() {
}

void Seance::OnWrite(/*file path*/) {}

void Seance::OnRead(/*file path*/) {}

void UserInterface::Input(Seance& C) {

  if (!sysh->Active()) {
    return;
  }

  sysh->Inputs(*kmap->uinputs);
  
  if (kmap->uinputs->IsEvent) {
    kmap->evaluate(&C.threads);
    UIroot->ProcEvent(&C, kmap->uinputs->Cursor);
  }

}

UserInterface::~UserInterface() {
  DEL(UIItem, UIroot);
  DEL(SysHandler, sysh);
  DEL(KeyMap, kmap);
}

void UserInterface::Output() {

  if (kmap->uinputs->IsEvent) {
    UIroot->Draw(nullptr);
    sysh->Output(UIroot);
  }
}