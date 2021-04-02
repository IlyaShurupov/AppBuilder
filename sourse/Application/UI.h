
#include "Types.h"
#include "Object.h"

struct UI : Obj {

    UI(Obj* prnt);

    static void PumpRequests(Method* me);
    static void Output(Method* me);
};
