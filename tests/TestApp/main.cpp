
#include "App.h"



int main () {
    // props are not copied correctly

    Obj* root = new Obj(nullptr, ObjType("root"));
    Application& app = ADDOBJ(Application, "app", *root, (root));
    app.Compose();
    app.Run();
}