
#include "ObjMacros.h"
#include "Types.h"


DEFINE_OBJ(List) {

    CONSTR(List) {
        DECL_BOOL(val) = false;
        DECL_FUNC(BOOL, Enter, ());
    }

    DEF_METHOD(Enter) {
        GET_THIS(List);
        USING_VAL(val);

        val = false;
    }
};

DEFINE_OBJ(Programm) {

    CONSTR(Programm) {
        DECL_CHILD(List, mychild);

        DECL_FLOAT(val) = 4.5f;
        DECL_LINK(Many, me) = this;

        
    }

    
    bool run() {
        Value status = Value(BOOL, "status"); 
        Get("mychild").Link()->Call(&status, "Enter", Args());
        return 0;
    }
};


int main () {
    COMPILE_PROGRAMM(Programm);  
    Programm->run();
}