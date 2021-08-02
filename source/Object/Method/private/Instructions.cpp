
#include "Method/Instructions.h"

void CallOpInst::exec(Array<Tuple<Obj*, Str>>& scope) {

	Obj* func = scope[func_idx].t1;
	if (func) {
		Method* call_func = ((MethodObj*)func)->func;

		// pass arguments
		if (args.Len() == call_func->args_len) {
			for (int i = 0; i < args.Len(); i++) {
				call_func->scope[ARG_BY_IDX(i)].t1 = scope[args[i]].t1;
			}
		}
		else {
			throw - 1;
		}

		call_func->execute();

		// save return
		if (ret_idx != -1) {
			scope[ret_idx].t1 = call_func->scope[RETURN_IDX].t1;
		}
	}
}

void GetInst::exec(Array<Tuple<Obj*, Str>>& scope) {
	Obj* child = &scope[from_idx].t1->GetChld(key);

	if (!child) {
		throw - 1;
	}

	scope[save_to_idx].t1 = child;
}

void JumpInst::exec(Array<Tuple<Obj*, Str>>& scope) {
	*instruction_idx_ptr = jump_idx;
}
