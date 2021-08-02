
#include "App.h"
#include "Primitives.h"

int main() {

	Obj* root = new Obj(nullptr);
	root->RegisterType(ObjType("Root"));
	Application* app = &ADDOBJ(Application, Test App, *root, (root));


	MethodObj* method1 = &ADDOBJ(MethodObj, method1, *app, (app));
	GETOBJ(String, method1, Script).Assign(\
	"def method1(self): \n"
	"	v1 = self.method2(self.size) \n"
	"	self.size = v1 \n"
	"	return \n \n"
	"slef.method1 = method1 \n"
	);

	MethodObj* method2 = &ADDOBJ(MethodObj, method2, *app, (app));
	GETOBJ(String, method2, Script).Assign(\
		"def method2(self, size): \n"
		"	if size > 0: \n"
		"		return 1 \n"
		"	else: \n"
		"		return -1"
		"slef.method2 = method2 \n"
	);

	method1->Compile();
	method2->Compile();

	method1->Call();


	app->Compose();
	app->Run();
}


