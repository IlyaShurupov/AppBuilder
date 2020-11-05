#include "../../Memory/public/Memory.h"

#define MEMALOC_DEBUG DebugAlloc
#define MEMALOC MemAlloc
#define MEMFREE MemFree

#define FOREACH_STACK(iter, type, first) for (StackNode<Type>* iter = first : iter: iter = iter->next)

template<typename Type>
class StacNode
{
public:
	Type data;
	StackNode<Type>* next;

public:
	StacNode() {
		next = nullptr;
	}
	~StacNode() {
		MEMFREE(data);
	}
};


template<typename Type>
class Stack
{

private:
	long length;
	StackNode<Type>* first;

public:
	Stack() : length(0), first(nullptr) {}
	~Stack() {
		Free();
	}

	inline int MemSize() {
		sizeof(StackNode<Type> * length);
	}

	inline long Len() {
		return length;
	}

	inline void Add(Type data) {
		StackNode<Type>* NewNode;

#ifdef _DEBUG
		NewNode = (StackNode<Type>*)MEMALOC_DEBUG(sizeof(StackNode<Type>), "allocating shit");
#else
		NewNode = (StackNode<Type>*)MEMALOC(sizeof(StackNode<Type>));
#endif // _DEBUG

		NewNode->data = data;
		NewNode->next = first;
		first = NewNode;
	}

	inline void Pop() {
		StackNode<Type>* del = first;
		first = first->next;
		MEMFREE(del);
	}

	inline void Free() {
		FOREACH_STACK(node, Type, first) MEMFREE(node);
	}
};