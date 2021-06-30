
#include "App.h"

int app_entry(Application* app) {

	// lists are not copied properly 


	app->Compose();
	app->Run();
	return 0;
}


