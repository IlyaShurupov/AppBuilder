
#include "Method/Method.h"

void Method::execute() {
	for (int instruction_idx = 0; instruction_idx < bin.Len(); instruction_idx++) {
		try {
			bin[instruction_idx].exec(scope);
		}
		catch (int ercode) {
			return;
		}
	}
}

bool Method::compile(Str* script) {
	bin.Delete();
	scope.Delete();
}