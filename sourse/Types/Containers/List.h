#pragma once

#include "Memory/Allocators.h"
#include "Sort.h"
#include "Macros.h"


#include <cstdarg>

#define FOREACH(List, Type, i) for (Iterator<Type> i(List, 0); i < (List)->Len(); ++i)

template <class Type> class Iterator;
template <class Type, typename AllocatorPolicy = AllocatorFixedSize> class List;

template <typename Type>
class Node {
 public:
  Type* data;
  Node<Type>* next = nullptr;
  Node<Type>* prev = nullptr;

  Node(Type* p_data) { data = p_data; }

  Type* operator->() { return data; }

  void FreeData() { DEL(Type, data); }
};

template <typename Type, typename AllocatorPolicy>
class List {

  Node<Type>* first = nullptr;
  Node<Type>* last = nullptr;
  int length = 0;
  AllocatorPolicy alloc;

 public:
  bool recursive_free_on_destruction = true;

 public:
  inline Node<Type>* First() { return first; }
  inline Node<Type>* Last() { return last; }
  inline int Len() { return length; }

  void Attach(Node<Type>* node, Node<Type>* node_to) {
    if (node_to) {
      if (node_to->next) {
        node->next = node_to->next;
        node->next->prev = node;
      }
      node_to->next = node;
      node->prev = node_to;
      if (node_to == last) {
        last = node;
      }
    } else {
      if (first) {
        first->prev = node;
        node->next = first;
        first = node;
      } else {
        first = last = node;
      }
    }
    length++;
  }

  void Detach(Node<Type>* node) {
    if (node->next) {
      node->next = node->prev;
    }
    if (node->prev) {
      node->prev = node->next;
    }

    if (node == last) {
      last = last->prev;
    }
    if (node == first) {
      first = first->next;
    }

    length--;
  }

  Node<Type>* Find(int idx) {
    if (!First() || idx < 0 || idx > Len() - 1) {
      return nullptr;
    }
    Node<Type>* found = First();
    for (int i = 0; i != idx; i++) {
      found = found->next;
    }
    return found;
  }

  Node<Type>* Find(Type* data) {
    Node<Type>* found = First();
    for (int i = 0; data != found->data; i++) {
      if (!found->next) {
        return nullptr;
      }
      found = found->next;
    }
    return found;
  }

  void ForEach(void (*functor)(List<Type>* list, Node<Type>* node)) {
    for (Node<Type>* node = First(); node; node = node->next) {
      functor(this, node);
    }
  }

  template <typename SortPolicy = SortMerge>
  void Sort(bool (*compare)(Type& obj1, Type& obj2)) {
    SortPolicy SortP;

    Type** buffer = ALLOC_AR(Type*, length);

    FOREACH(this, Type, iter) { *(buffer + iter.Idx()) = iter.Data(); }

    SortP.Sort(buffer, length, compare);

    FOREACH(this, Type, iter) { iter.node()->data = *(buffer + iter.Idx()); }

    DEALLOC(buffer);
  }

  void Invert() {
    Iterator<Type> i(this, 0);
    Iterator<Type> j(this, Len() - 1);
    while (i < Len() / 2) {
      SWAP(i.node()->data, j.node()->data, Type*);
      ++i;
      --j;
    }
  }

  inline Type& operator[](Iterator<Type>& iter) { return *iter.node()->data; }
  inline Type& operator[](int idx) { return *Find(idx)->data; }

  void PushBack(Node<Type>* new_node) { Attach(new_node, Last()); }
  void PushFront(Node<Type>* new_node) { Attach(new_node, nullptr); }

  void PushBack(Type* data) { 
    PushBack(new (alloc.Get(sizeof(Node<Type>))) Node<Type>(data)); 
  }
  void PushFront(Type* data) { 
    PushFront(new (alloc.Get(sizeof(Node<Type>))) Node<Type>(data)); 
  }

  void Insert(Node<Type>* node, int idx) {
    Node<Type>* place_to = Find(idx);
    Attach(node, place_to->prev);
  }

  void Insert(Type* data, int idx) { 
    Insert(new (alloc.Get(sizeof(Node<Type>))) Node<Type>(data), idx); 
  }

  void DelNode(Node<Type>* node) {
    Detach(node);
    node->FreeData();
    alloc.Deallocate(node);
  }

  void Release() {
    ForEach([](List<Type>* list, Node<Type>* node) { list->Detach(node); });
  }

  void Delete() {
    ForEach([](List<Type>* list, Node<Type>* node) { list->DelNode(node); });
  }

  List<Type>& operator += (const List<Type>& in) {
    for (Node<Type>* node = in.first; node; node = node->next) {
      PushBack(node->data);
    }
    return *this;
  }

  List<Type>& operator = (const List<Type>& in) {

    if (recursive_free_on_destruction) {
      Delete();
    } else {
      Release();
    }
    
    *this += in;

    alloc = in.alloc;
    recursive_free_on_destruction = in.recursive_free_on_destruction;

    return *this;
  }

  template <typename compare_val>
  Node<Type>* Find(bool (*found)(Node<Type>* node, compare_val val), compare_val value) {
    for (Node<Type>* node = First(); node; node = node->next) {
      if (found(node, value)) {
        return node;
      }
    }
    return nullptr;
  }

  List() {}
  List(bool recursive_free_on_destruction) : recursive_free_on_destruction(recursive_free_on_destruction) {}
  
  ~List() {
    if (recursive_free_on_destruction) {
      Delete();
    } else {
      Release();
    }
  }
};

template <typename Type>
class Iterator {

  Node<Type>* iter;
  int idx;

 public:
  int Idx() { return idx; }
  Type* operator->() { return iter->data; }
  Type* Data() { return iter->data; }
  Node<Type>* node() { return iter; }

  Iterator(List<Type>* list, int p_idx) {
    idx = p_idx;
    iter = list->Find(idx);
  }

  inline void operator++() {
    iter = iter->next;
    idx++;
  }

  inline void operator--() {
    iter = iter->prev;
    idx--;
  }

  bool operator==(Iterator<Type> IterNode) { return IterNode.iter == iter; }

  bool operator==(int p_idx) { return idx == p_idx; }
  bool operator!=(int p_idx) { return idx != p_idx; }
  bool operator>(int p_idx) { return idx > p_idx; }
  bool operator<(int p_idx) { return idx < p_idx; }
  bool operator>=(int p_idx) { return idx >= p_idx; }
  bool operator<=(int p_idx) { return idx <= p_idx; }

};