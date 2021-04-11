
#include "App.h"

void test () {

}

int main () {

    // test();
    // lists are not copied properly 

    Obj* root = new Obj(nullptr);
    Application& app = ADDOBJ(Application, "app", *root, (root));
    app.Compose();
    app.Run();
}


