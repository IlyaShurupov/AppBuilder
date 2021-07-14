
#include "Compiler/CCompiler.h"

#include <libtcc.h>

DynFunc::DynFunc() {}

void DynFunc::init() {
    state = tcc_new();
    
    tcc_set_output_type((TCCState *)state, TCC_OUTPUT_MEMORY);
    
    //tcc_add_library_path((TCCState*)state, tcclibpath);
}

void DynFunc::free() {
    if (state) {
        tcc_delete((TCCState*)state);
        state = nullptr;
    }
}

bool DynFunc::compile() {

    free();
    init();

    if (tcc_compile_string((TCCState*)state, program.str) > 0) {
        return false;
    }

    tcc_relocate((TCCState*)state, TCC_RELOCATE_AUTO);
    return true;
}

void* DynFunc::get_func(const Str& symbol) {
    return tcc_get_symbol((TCCState*)state, symbol.str);
}

DynFunc::~DynFunc() {}
