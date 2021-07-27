
#include "App.h"
#include "Primitives.h"


void test(Application* app) {

	Method* method = &ADDOBJ(Method, method, *app, (app));
	String* script = &GETOBJ(String, method, Script);

	Int* integer = &ADDOBJ(Int, Integer, *method, (method));

	script->Assign( \
		"#include \"Object.h\" \n"
		"#include \"Primitives.h\" \n"
		"extern \"C\" { \n"
		"__declspec(dllexport) void __start() {} \n"
		"__declspec(dllexport) void entry(Obj *self) { GETOBJ(Int, self, Integer).Set(1); } \n"
		"__declspec(dllexport) void __end() {} \n"
		"} \n"
	);

	method->Compile();
	method->Call();

	int g = integer->GetVal();
}


int main() {

	Obj* root = new Obj(nullptr);
	root->RegisterType(ObjType("Root"));

	Application* app = &ADDOBJ(Application, Test App, *root, (root));

	test(app);

	app->Compose();
	app->Run();

	return 0;
}


