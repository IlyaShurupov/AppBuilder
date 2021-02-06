#pragma once

#include "Mem.h"

#define FOREACH_NODE(NodeType, List, iter_node) \
  for (Node<NodeType>* iter_node = &List->first(); iter_node; iter_node = iter_node->Next)

#define FOREACH(List, Type, node) for (Node<Type>* node = &(List)->first(); node; node = node->Next)

#define DO_FOREACH_IF(action, type, list, cond)                                         \
  for (Node<type>* iter_node = &list.first(); iter_node; iter_node = iter_node->Next) { \
    type* iter = iter_node->Data;                                                       \
    if (cond)                                                                           \
      action;                                                                           \
  }

#define FOREACH_DO(list, type, action)                                                  \
  for (Node<type>* iter_node = &list.first(); iter_node; iter_node = iter_node->Next) { \
    type* iter = iter_node->Data;                                                       \
    action;                                                                             \
  }

template <typename Type>
class Node {
 public:
  Type* Data;

  Node<Type>* Next;
  Node<Type>* Prev;
  size_t idx;

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

  void free() { DELETE_DBG(Type, Data); }
  ~Node() {}
};

template <typename Type>
class List {
 private:
  Node<Type>* First;
  Node<Type>* Last;
  Node<Type>* Buff;
  size_t length;

  Node<Type>* SearchNode(size_t index, Type* data);

 public:
  List();
  ~List();
  void add(Type* data);
  Type* operator[](size_t idx);
  size_t len();

  void del();

  void pop(bool recursice);
  void del(Type* node_data);
  void del(size_t idx, bool recursice);
  void del(size_t idx_start, size_t idx_end, bool recursice);
  void del(Node<Type>* node, bool recursice);

  void pop();
  void del(size_t idx);
  void del(size_t idx_start, size_t idx_end);
  void del(Node<Type>* node);

  void popnode();
  void delnode(size_t idx);
  void delnode(size_t idx_start, size_t idx_end);
  void delnode(Node<Type>* node);
  void release();

  Node<Type>& first();
  Node<Type>& last();
};

template <typename Type>
List<Type>::List() {
  First = nullptr;
  Last = nullptr;
  Buff = nullptr;
  length = 0;
}

template <typename Type>
void List<Type>::add(Type* data) {
  if (!length) {
    First = NEW_DBG(Node<Type>) Node<Type>(Last, nullptr, length, data);
    Last = First;
  } else {
    Last->Next = NEW_DBG(Node<Type>) Node<Type>(Last, nullptr, length, data);
    Last = Last->Next;
  }
  length += 1;
}

template <typename Type>
Type* List<Type>::operator[](size_t idx) {
  Node<Type>* item = SearchNode(idx, nullptr);
  Buff = item->Next;
  return item->Data;
}

template <typename Type>
Node<Type>* List<Type>::SearchNode(size_t index, Type* data) {

  if (Buff && index == Buff->idx) {
    return Buff;
  }

  bool invert = index > length / 2;
  Node<Type>* item = (&First)[invert];

  // search by data
  if (data) {
    if (!invert) {
      while (item->Data != data) {
        item = item->Next;
      }
    } else {
      while (item->Data != data) {
        item = item->Prev;
      }
    }

  } else {
    // search by idx
    if (!invert) {
      while (item->idx != index) {
        item = item->Next;
      }
    } else {
      while (item->idx != index) {
        item = item->Prev;
      }
    }
  }

  return item;
}

template <typename Type>
void List<Type>::del(size_t index_start, size_t index_end, bool recursice) {

  Node<Type>* first = SearchNode(index_start, nullptr);
  Node<Type>* last = SearchNode(index_end, nullptr);
  Node<Type>* next = last->Next;
  Node<Type>* prev = first->Prev;

  size_t idx_diff = index_end - index_start;

  // Deleting Items
  Node<Type>* del_node;
  Buff = first;
  for (size_t i = index_start; i <= index_end; i++) {
    del_node = Buff;
    Buff = Buff->Next;
    if (recursice) {
      del_node->free();
    }
    DELETE_DBG(Node<Type>, del_node);
  }

  // Reconecting Links
  if (next && prev) {
    next->Prev = prev;
    prev->Next = next;
  } else {
    if (!(next || prev)) {
      Last = First = nullptr;
    } else {
      if (!next) {
        prev->Next = nullptr;
        Last = prev;
      } else {
        next->Prev = nullptr;
        First = next;
      }
    }
  }

  // Changing indexes
  if (next) {
    for (size_t i = index_end + 1; i < length - 1; i++) {
      next->idx -= (idx_diff + 1);
      next = next->Next;
    }
  }

  length -= (idx_diff + 1);
}

template <typename Type>
void List<Type>::del(size_t index, bool recursice) {

  Node<Type>* del_node = SearchNode(index, nullptr);

  this->del(del_node, recursice);
}

template <typename Type>
void List<Type>::del(Node<Type>* node, bool recursice) {
  Node<Type>* del_node = node;

  if (del_node->Next) {
    del_node->Next->Prev = del_node->Prev;

    Buff = del_node;

    for (size_t i = del_node->idx; i < length; i++) {
      Buff->idx -= 1;
      Buff = Buff->Next;
    }
    Buff = del_node->Next;
  }
  if (del_node->Prev) {
    del_node->Prev->Next = del_node->Next;
  }

  if (del_node == this->First) {
    this->First = del_node->Next;
  }

  if (del_node == this->Last) {
    this->Last = del_node->Prev;
  }

  if (recursice) {
    del_node->free();
  }
  DELETE_DBG(Node<Type>, del_node);

  length -= 1;
}

template <typename Type>
void List<Type>::pop(bool recursice) {
  Node<Type>* prev_item = Last->Prev;
  if (recursice) {
    Last->free();
  }
  DELETE_DBG(Node<Type>, Last);
  Last = prev_item;
  prev_item->Next = 0;
  length -= 1;
}

template <typename Type>
void List<Type>::del(Type* node_data) {
  Node<Type>* del_node = SearchNode(0, node_data);
  this->delnode(del_node);
}

//  -------------------------

template <typename Type>
void List<Type>::del(size_t index_start, size_t index_end) {
  del(index_start, index_end, true);
}

template <typename Type>
void List<Type>::del(size_t index) {
  del(index, true);
}

template <typename Type>
void List<Type>::del(Node<Type>* node) {
  del(node, true);
}

template <typename Type>
void List<Type>::pop() {
  pop(true);
}

//  -------------------------

template <typename Type>
void List<Type>::delnode(size_t index_start, size_t index_end) {
  del(index_start, index_end, false);
}

template <typename Type>
void List<Type>::delnode(size_t index) {
  del(index, false);
}

template <typename Type>
void List<Type>::delnode(Node<Type>* node) {
  del(node, false);
}

template <typename Type>
inline void List<Type>::release() {
  del(0, length, false);
}

template <typename Type>
void List<Type>::popnode() {
  pop(false);
}

//  -------------------------

template <typename Type>
Node<Type>& List<Type>::last() {
  return *this->Last;
}

template <typename Type>
Node<Type>& List<Type>::first() {
  return *this->First;
}

template <typename Type>
size_t List<Type>::len() {
  return length;
}

template <typename Type>
inline void List<Type>::del() {
  if (length) {
    del(0, length - 1, true);
  }
}

template <typename Type>
List<Type>::~List() {
  release();
}