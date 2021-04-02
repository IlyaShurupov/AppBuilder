
#include "UI.h"


UI::UI(Obj* prnt) : Obj(prnt, "UI") {
    AddMethod("PumpRequests") = PumpRequests;
    AddMethod("Output") = Output;
}

void UI::PumpRequests(Method* me) {

}

void UI::Output(Method* me) {

}
