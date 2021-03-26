
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

void sdltest();

int main__() {


  Application DrawApp(60);

  DrawApp.AddOperator((Operator*)NEW(OpLogHeap)());
  DrawApp.AddOperator((Operator*)NEW(OpConsoleToggle)());
  DrawApp.AddOperator((Operator*)NEW(OpEndSeance)());
  DrawApp.AddOperator((Operator*)NEW(OpMoveCanvas)());
  DrawApp.AddOperator((Operator*)NEW(OpUIIMove)());
  DrawApp.AddOperator((Operator*)NEW(OpUIIResize)());
  
  Str path("/home/ilusha/dev/AppBuilder/build/bin/Linux64/DrawApp/Configuration/");
  DrawApp.LoadConfiguration(&path, resolveUiRef);

  DrawApp.Launch();
  return 0;
}

#include "../../sourse/Device/Device.h"
#include "../../sourse/Device/Texture.h"

int main(int argc, char *argv[]) {

	DeviceManager dev;
	DevBuffer tex(200, 200);
	DevBuffer tex2(100, 100);

	tex.DrawRect(Rect<int>(0, 0, 200, 200), Color(1, 1, 0, 1));
	tex2.DrawRect(Rect<int>(0, 0, 100, 100), Color(0, 1, 0, 1));

	tex.Project(&tex2, vec2<int>(0, 0));


	tex.draw_tex_ro_rend(vec2<int>(50, 50));
	

	SDL_Delay(2000);
	

	return 0;
}
