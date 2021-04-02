# pragma once

#include "Types.h"
#include "Object.h"

struct Operator : Obj {
    
    Operator();

    static void Poll(Method* me);
    static void Invoke(Method* me);
    static void Modal(Method* me);
};
