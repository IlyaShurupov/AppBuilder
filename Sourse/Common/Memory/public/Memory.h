#pragma once

#include <stdlib.h>
#include <iostream>
#include <cassert>

#define MAX_DESCRIBLION_LENGTH 16
#define MEMHEAD_FROM_PTR(ptr) (((MemHead *)ptr) - 1)
#define PTR_FROM_MEMHEAD(memhead) (memhead + 1)
#define SIZET_ALIGN_4(len) ((len + 3) & ~(size_t)3)

struct MemHead {
	MemHead* prev = nullptr;
	MemHead* next = nullptr;
	char description[MAX_DESCRIBLION_LENGTH];
};


size_t getSize();
bool MemCheckLeaks(bool Error);
void AddInList(MemHead* mem_head, const char* str);
void RemoveFromList(MemHead* mem_head);

inline void MemFree(void* ptr) {

#ifdef _DEBUG
	MemHead* mem_head = MEMHEAD_FROM_PTR(ptr);
	RemoveFromList(mem_head);
	free(mem_head);
#endif // DEBUG

	free(ptr);
}

inline void* MemAlloc(size_t size, const char description[MAX_DESCRIBLION_LENGTH]) {

#ifdef _DEBUG
	MemHead* mem_head = (MemHead*)malloc(size + sizeof(MemHead));
	AddInList(mem_head, description);
	return PTR_FROM_MEMHEAD(mem_head);
#endif
	return malloc(size);
}