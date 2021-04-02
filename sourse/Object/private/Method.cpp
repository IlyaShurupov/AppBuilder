
#include "Method.h"

#include "Object.h"

Method::Method(Obj* p_prnt) {
    prnt = p_prnt;
}

bool Method::call() {

    if (method_body) {
        // check args, ret type
        method_body(this);
    }
    
    return false;
}