#include "public/Memory.h"

#define MEMHEAD_FROM_PTR(ptr) (((MemHead *)ptr) - 1)
#define PTR_FROM_MEMHEAD(memhead) (memhead + 1)

#define MAXDESCRIPTIONSIZE 16

struct MemHead {
	std::string description;
	MemHead* prev = nullptr;
	MemHead* next = nullptr;
};

typedef struct MemDebug {
	size_t size = 0;
	MemHead* entry = nullptr;
} MemDebug;

MemDebug Mem;

size_t getSize() {
	return Mem.size;
}

bool CheckLeaks(bool Error) {

	if (Mem.entry) {
		printf("MEMORY LEAKS DETECTED!!!\n");

		if (Mem.entry->next) {
			MemHead* iter = Mem.entry;
			while (iter)
			{
				std::cout << iter->description << "\n";
				iter = iter->next;
			}
		}
		else if (Mem.entry->prev) {
			MemHead* iter = Mem.entry;
			while (iter)
			{
				std::cout << iter->description << "\n";
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


inline static void AddInList(MemHead* mem_head, std::string* str) {
	Mem.entry->next = mem_head;
	mem_head->prev = Mem.entry;
	mem_head->next = nullptr;
	Mem.entry = mem_head;
	mem_head->description = *str;
}

inline static void RemoveFromList(MemHead* mem_head) {

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

inline void MemFree(void* ptr) {

#ifdef _DEBUG
	MemHead* mem_head = MEMHEAD_FROM_PTR(ptr);
	RemoveFromList(mem_head);
	free(mem_head);
#endif // DEBUG


	free(ptr);
}

inline void* DebugAlloc(size_t size, std::string str) {
	MemHead* mem_head = (MemHead*)malloc(sizeof(MemHead) + size);
	AddInList(mem_head, &str);
	return PTR_FROM_MEMHEAD(mem_head);
}

inline void* MemAlloc(size_t size) {
	return malloc(size);
}