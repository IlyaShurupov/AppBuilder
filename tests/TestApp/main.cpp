
#include "App.h"


int main () {

    // props are not copied correctly
    // requester should have multipule threads
    // no reference of "INVOKED" "EXECUTED" et outside operator class - use getters instead

    Obj* root = new Obj(nullptr, ObjType("root"));
    Application& app = Application::Add(root, "app");
    app.Compose();
    app.Run();
}