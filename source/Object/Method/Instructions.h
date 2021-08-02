#pragma once

#include "Method/Method.h"

#include "Containers/Array.h"
#include "Containers/Tuple.h"

struct Instruction {
	virtual void exec(Array<Tuple<Obj*, Str>>& scope) {}
};

struct CallOpInst : Instruction {

	Array<int> args;

	int func_idx = 0;
	int ret_idx = 0;

	void exec(Array<Tuple<Obj*, Str>>& scope);
};

struct GetInst : Instruction {

	Str key;
	int from_idx = 0;
	int save_to_idx = 0;

	void exec(Array<Tuple<Obj*, Str>>& scope);
};

struct JumpInst : Instruction {

	int* instruction_idx_ptr = nullptr;
	int jump_idx;

	void exec(Array<Tuple<Obj*, Str>>& scope);
};