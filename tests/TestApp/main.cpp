
#include "App.h"


int main () {
    Obj* root = new Obj(nullptr, "root");
    Application::Add(root, "app").Run();
}