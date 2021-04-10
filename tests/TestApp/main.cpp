
#include "App.h"

void test () {

}

int main () {

    // test();

    Obj* root = new Obj(nullptr);
    Application& app = ADDOBJ(Application, "app", *root, (root));
    app.Compose();
    app.Run();
}


