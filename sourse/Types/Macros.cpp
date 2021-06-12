
#include "Macros.h"

int FltToInt(float f) {
	union { float f; aligned i; } u;
	u.f = f;
	return u.i;
}

float IntToFlt(aligned i) {
	union { float f; aligned i; } u;
	u.i = i;
	return u.f;
}

uint8 next_pow_of_2(uint8 v) {
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	return v + 1;
}

int hash_string(const char* bytes) {
	unsigned long hash = 5381;
	int c;
	while ((c = *bytes++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}