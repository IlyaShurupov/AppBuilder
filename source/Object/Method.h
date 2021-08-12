#pragma once

/*

#include "Primitives.h"

#include "Containers/Array.h"
#include "Containers/Tuple.h"

struct Instruction {
	void (*opcode)(Array<int>* idx_args, Array<Tuple<Obj*, Str>>* scope);
	Array<int> idx_args;
	
	void exec(Array<Tuple<Obj*, Str>>* scope) {
		opcode(&idx_args, scope);
	}
};

class Func {

public:

	Str script;

	List<Instruction> bin;
	Array<Tuple<Obj*, Str>> scope;
	int args_len = 0;

	void execute() {
		for (auto instruction : bin) {
			try {
				instruction->exec(&scope);
			}
			catch (int ercode) {
				return;
			}
		}
	}

	bool compile() {
		bin.Delete();
		scope.Free();


	}
};

void opcode_call(Array<int>* idx_args, Array<Tuple<Obj*, Str>>* scope) {
	Obj* func = (*scope)[(*idx_args)[0]].t1;
	if (func) {
		Func* call_func = ((Method*)func)->func;

		// pass arguments
		if (idx_args->Len() - 2 == call_func->args_len) {
			for (int i = 2; i < idx_args->Len(); i++) {
				call_func->scope[i - 1].t1 = (*scope)[(*idx_args)[i]].t1;
			}
		}
		else {
			throw - 1;
		}

		call_func->execute();

		// save return
		if ((*idx_args)[1] != -1) {
			(*scope)[(*idx_args)[1]].t1 = call_func->scope[0].t1;
		}
	}
}

*/