

#include "Thread/Thread.h"

Operator::Operator(const Operator& in) : Obj(in) {
    instance_count = in.instance_count;
    parallel = in.parallel; 
    state = in.state;
} 


Operator::Operator(Obj* prnt) : Obj(prnt) { 
    RegisterType("Operator");
    
    ObDict& args = ADDOBJ(ObDict, Interface, *this, (this)).Assign("Obj", true);
    args.AddObj(new String(&args), "Run Time Arg");
    
    ADDOBJ(Link, Args, *this, (this)).Init("Obj", true);
}


bool Operator::Finished() { return state == OpState::FINISHED; }


