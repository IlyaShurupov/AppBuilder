
#include "App.h"

void test () {

}

int main () {

    // test();
    // lists are not copied properly 

    Obj* root = new Obj(nullptr);
    root->RegisterType(ObjType("Root"));

    Application& app = ADDOBJ(Application, Test App, *root, (root));
    app.Compose();
    app.Run();
    return 0;
}


