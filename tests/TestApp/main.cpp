
#include "App.h"

int app_entry(Application* app) {

	// lists are not copied properly 

	DynFunc func;

	func.program = "int foo() {return 1;}";
	func.compile();
	int (*foo)() = (int (*)())func.get_func("foo");
	int g = foo();

	app->Compose();
	app->Run();
	return 0;
}


