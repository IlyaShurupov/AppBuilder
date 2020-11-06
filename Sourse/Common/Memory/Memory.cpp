#include "public/Memory.h"

#define PRINT_LEAK(iter) std::cout << " '" << iter->description << "' " << "\n"

typedef struct MemDebug {
	size_t size = 0;
	MemHead* entry = nullptr;
} MemDebug;

MemDebug Mem;

size_t getSize() {
	return Mem.size;
}

void AddInList(MemHead* mem_head, const char * str) {

	for (int i = 0; i < MAX_DESCRIBLION_LENGTH; i++) {
		mem_head->description[i] = str[i];
	}

	mem_head->next = nullptr;

	if (Mem.entry) {
		Mem.entry->next = mem_head;
		mem_head->prev = Mem.entry;
	}
	else {
		mem_head->prev = nullptr;
	}

	Mem.entry = mem_head;
}

void RemoveFromList(MemHead* mem_head) {

	if (mem_head->next) {
		mem_head->next->prev = mem_head->prev;
	}

	if (mem_head->prev) {
		mem_head->prev->next = mem_head->next;
	}

	if (mem_head == Mem.entry) {
		Mem.entry = mem_head->prev;
	}
}

bool MemCheckLeaks(bool Error = false) {

	if (Mem.entry) {
		printf("MEMORY LEAKS DETECTED!!!\n");

		if (Mem.entry->next) {
			MemHead* iter = Mem.entry;
			while (iter)
			{
				PRINT_LEAK(iter);
				iter = iter->next;
			}
		}
		else if (Mem.entry->prev) {
			MemHead* iter = Mem.entry;
			while (iter)
			{
				PRINT_LEAK(iter);
				iter = iter->prev;
			}
		}

		if (Error) {
			assert(0);
		}

		return true;
	}
	return false;
}