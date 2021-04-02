
#include "Method.h"

#include "Object.h"

Method::Method(Obj* p_prnt, ValType p_out, const ArgTypes& p_arg_types) {
    prnt = p_prnt;
    type_out = p_out;
    arg_types = ArgTypes(p_arg_types);
}