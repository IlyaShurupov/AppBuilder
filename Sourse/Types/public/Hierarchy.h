#pragma once

// idx tmpl param is the idx of hrchy member in the struct
template <typename Type, typename ListType, int hrch_idx>
struct Hierarchy {
  ListType childs;
  Type* prnt = nullptr;
  Hierarchy() {
  }
  Hierarchy(Type* parent) {
    join(parent);
  }
  void add(Type* chld) {
    childs.add(chld);
    ((Hierarchy*)chld + hrch_idx)->prnt = (Type*)(this - hrch_idx);
  }
  void join(Type* parent) {
    ((Hierarchy*)parent + hrch_idx)->childs.add((Type*)(this - hrch_idx));
    this->prnt = parent;
  }
  void leave() {
    if (prnt) {
      ((Hierarchy*)prnt + hrch_idx)->childs.del((Type*)(this - hrch_idx));
    }
  }
  Type* root() {
    Hierarchy* root = (Hierarchy*)prnt;
    while (true) {
      if ((root + hrch_idx)->prnt) {
        root = (Hierarchy*)(root + hrch_idx)->prnt;
        continue;
      }
      break;
    }
    return (Type*)root;
  }
};