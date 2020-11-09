#pragma once

#include "Memory.h"

#define FOREACH_STACK(iter, type, first) for (StackNode<Type>* iter = first ; iter; iter = iter->next)

template<typename Type>
class StackNode
{
public:
	StackNode<Type>* next;
	Type* data;

public:
	StackNode(Type* data, StackNode<Type>* next) {
		this->next = next;
		this->data = data;
	}
	~StackNode() {
	}
};

template<typename Type>
class Stack
{

private:
	long length;
	StackNode<Type>* first;

public:


	Stack() {
		length = 0;
		first = nullptr;
	}
	~Stack() {
		Free();
	}

	inline int MemSize() {
		return sizeof(StackNode<Type>) * length;
	}

	inline long Len() {
		return length;
	}

	inline void Add(Type& data) {
		StackNode<Type>* NewNode = new StackNode<Type>(&data, first);
		first = NewNode;
		length++;
	}

	inline void Pop() {
		StackNode<Type>* del = first;
		first = first->next;
		delete del;
		length--;
	}

	inline void Free() {
		StackNode<Type>* del = first;
		for (size_t i = 0; i < length; i++) {
			StackNode<Type>* next = del->next;
			delete del;
			del = next;
		}
	}
};