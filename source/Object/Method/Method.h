#pragma once

#include "Primitives.h"

#include "Method/Instructions.h"

#define LOCALS_OFFSET 3
#define RETURN_IDX 0
#define SELF_IDX 1
#define TMP_IDX 2

#define ARG_BY_IDX(idx) idx + LOCALS_OFFSET

struct Method {

	DictObj defs;
	Array<Instruction> bin;
	Array<Tuple<Obj*, Str>> scope;
	int args_len = 0;

	int exec_idx = 0;

	void execute();

	bool compile(Str* script);
};
