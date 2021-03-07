
#include "Application.h"

#include "Operators.h"
#include "UInterfaces.h"

UIItem* resolveUiRef(Str* name, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb) {
  UIItem* newuii = nullptr;
  if (*name == Str("Button")) {
    newuii = (UIItem*)NEW(UIIButton)(ops, paramsdb, uiidb);
  } else if (*name == Str("Group")) {
    newuii = (UIItem*)NEW(UIIGroup)(ops, paramsdb, uiidb);
  }
  return newuii;
}

int main() {

  Application DrawApp(60);

  DrawApp.AddOperator((Operator*)NEW(OpLogHeap)());
  DrawApp.AddOperator((Operator*)NEW(OpConsoleToggle)());
  DrawApp.AddOperator((Operator*)NEW(OpEndSeance)());
  DrawApp.AddOperator((Operator*)NEW(OpMoveCanvas)());
  DrawApp.AddOperator((Operator*)NEW(OpUIIMove)());
  DrawApp.AddOperator((Operator*)NEW(OpUIIResize)());
  
  Str path(getExecutablePath());
  path.trim(Range(0, path.rfind('\\', Range(0, path.length))));
  path += Str("Configuration\\");
  DrawApp.LoadConfiguration(&path, resolveUiRef);

  DrawApp.Launch();
}
