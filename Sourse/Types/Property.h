#pragma once

#include "Containers/LinkedList.h"
#include "Strings.h"

class PropertyInt {
 public:
  Str idname;
  Str describtion;

  PropertyInt();
  PropertyInt(int val, int min, int max);
  ~PropertyInt();

  void setVal(int val);
  void setMin(int min);
  void setMax(int max);
  int get();

  int val;
  int min, max;
};

class PropertyFloat {
 public:
  Str idname;
  Str describtion;

  PropertyFloat();
  PropertyFloat(float val, float min, float max);
  ~PropertyFloat();

  void assign(float val, float min, float max);

  void setVal(float val);
  void setMin(float min);
  void setMax(float max);
  float get();

  float val;
  float min, max;
};


class PropertyObjectPtr {
 public:
  Str idname;
  void* obj;

  void assign(void* obj_ptr);
  PropertyObjectPtr();
  ~PropertyObjectPtr();
};

class PropertyBuffPtr {
 public:
  Str idname;
  void* buff;

  void assign(void* buff_ptr);
  PropertyBuffPtr();
  ~PropertyBuffPtr();
};

class PropertyFuncAdress {
 public:
  void* (*func)(void* arg) = nullptr;

  PropertyFuncAdress();
  ~PropertyFuncAdress();
};

struct Properties {
  List<PropertyInt> Ints;
  List<PropertyFloat> Floats;
  List<PropertyObjectPtr> Pointers_Obj;
  List<PropertyBuffPtr> Pointers_Buff;
  ~Properties();
};