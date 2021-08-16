
#include "App.h"
#include "Primitives.h"

int main() {

	mem_test_types();

	Obj* root = new Obj(nullptr);
	root->RegisterType(ObjType("Root"));

	Application* app = &ADDOBJ(Application, Test App, *root, (root));

	//Method* method = &ADDOBJ(Method, method, *app, (app));

	app->Compose();
	app->Run();


	Obj* s = new Obj(0);

	return 0;
}


