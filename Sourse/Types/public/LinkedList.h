#pragma once
#include <cassert>

#define FOREACH_NODE(NodeType, List, iter_node) for (Node<NodeType>* iter_node = &List->first(); iter_node; iter_node = iter_node->Next)

template<typename Type>
class Node
{
public:
	Type* Data;

	Node<Type>* Next;
	Node<Type>* Prev;
	int idx;

public:

	Node() {
		Next = nullptr;
		Prev = nullptr;
		idx = -1;
	}

	Node(Node<Type>* prev, Node<Type>* next, size_t idx, Type* data) {
		this->Data = data;
		this->Next = next;
		this->Prev = prev;
		this->idx = idx;
	}

	~Node() {
	}
};

template<typename Type>
class List
{
private:

	Node<Type>* First;
	Node<Type>* Last;
	Node<Type>* Buff;
	size_t length;

	Node<Type>* SearchNode(size_t index);

public:
	List();
	~List();
	void add(Type& data);
	Type* operator[](size_t idx);
	void pop();
	size_t len();
	void del(size_t idx);
	void del(size_t idx_start, size_t idx_end);
	Node<Type>& first();
	Node<Type>& last();
};

template<typename Type>
List<Type>::List() {
	First = nullptr;
	Last = nullptr;
	Buff = nullptr;
	length = 0;
}

template<typename Type>
void List<Type>::add(Type& data)
{
	if (!length)
	{
		First = new Node<Type>(Last, nullptr, length, &data);
		Last = First;
	}
	else
	{
		Last->Next = new Node<Type>(Last, nullptr, length, &data);
		Last = Last->Next;
	}
	length += 1;
}

template<typename Type>
Type* List<Type>::operator[](size_t idx)
{
	Node<Type>* item = SearchNode(idx);
	Buff = item->Next;
	return item->Data;
}

template<typename Type>
Node<Type>* List<Type>::SearchNode(size_t index)
{
	assert(index < length);

	if (Buff && index == Buff->idx) {
		return Buff;
	}

	bool invert = index > length / 2;
	Node<Type>* item = (&First)[invert];

	if (!invert) {
		while (item->idx != index) {
			item = item->Next;
		}
	}
	else {
		while (item->idx != index) {
			item = item->Prev;
		}
	}

	return item;
}

template<typename Type>
void List<Type>::del(size_t index_start, size_t index_end) {

	assert(index_end < length);
	
	Node<Type>* first = SearchNode(index_start);
	Node<Type>* last = SearchNode(index_end);
	Node<Type>* next = last->Next;
	Node<Type>* prev = first->Prev;

	size_t idx_diff = index_end - index_start;

	// Deleting Items
	Node<Type>* del_node;
	Buff = first;
	for (int i = index_start; i <= index_end; i++) {
		del_node = Buff;
		Buff = Buff->Next;
		delete del_node;
	}

	// Reconecting Links
	if (next && prev) {
		next->Prev = prev;
		prev->Next = next;
	}
	else {
		if (!(next || prev)) {
			Last = First = nullptr;
		}
		else {
			if (!next) {
				prev->Next = nullptr;
				Last = prev;
			}
			else {
				next->Prev = nullptr;
				First = next;
			}
		}
	}

	//Changing indexes
	if (next) {
		for (int i = index_end + 1; i < length - 1; i++) {
			next->idx -= (idx_diff + 1);
			next = next->Next;
		}
	}

	length -= (idx_diff + 1);
}

template<typename Type>
Node<Type>& List<Type>::last()
{
	return *this->Last;
}

template<typename Type>
Node<Type>& List<Type>::first()
{
	return *this->First;
}

template<typename Type>
void List<Type>::del(size_t index) {

	assert(index < length);

	Node<Type>* del_node = SearchNode(index);

	if (del_node->Next) {
		del_node->Next->Prev = del_node->Prev;

		Buff = del_node;
		for (size_t i = index; i < length; i++) {
			Buff->idx -= 1;
			Buff = Buff->Next;
		}
		Buff = del_node->Next;
	}
	if (del_node->Prev) {
		del_node->Prev->Next = del_node->Next;
	}

	delete del_node;

	length -= 1;
}

template<typename Type>
void List<Type>::pop() {
	Node<Type>* prev_item = Last->Prev;
	delete Last;
	Last = prev_item;
	prev_item->Next = 0;
	length -= 1;
}

template<typename Type>
size_t List<Type>::len() {
	return length;
}

template<typename Type>
List<Type>::~List() {
	del(length - 1);
}


