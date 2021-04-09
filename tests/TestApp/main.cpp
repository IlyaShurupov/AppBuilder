
#include "App.h"

void test () {

    Int val1(nullptr);
    val1.Assign(0, 0, 0);
    Int* i2 = &(Int&)((Obj &)val1).Instance();
    ObjType* type = &i2->type;
}

int main () {

    test();
    // props are not copied correctly

    Obj* root = new Obj(nullptr);
    Application& app = ADDOBJ(Application, "app", *root, (root));
    app.Compose();
    app.Run();
}


