#pragma once

#include "Types.h"
#include "Object.h"

#include "Operator.h"
#include "UI.h"

struct Application : Obj {

    Application();

    static void CoreLoop(Method* cd);
};
