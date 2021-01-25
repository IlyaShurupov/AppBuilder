#pragma once

//#include "FrameBuff.h"
#include "LinkedList.h"
#include "MathMacros.h"
//#include "Object.h"

// TODO: a lot to do

class PropertyInt {
 public:
  char name[10];
  char describtion[10];

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
  char name[10];
  char describtion[10];

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
  char name[10];

  //class Object* obj_ptr;
  void* obj;
  void assign(void* obj_ptr);

  PropertyObjectPtr();
  ~PropertyObjectPtr();
};

class PropertyBuffPtr {
  public:
  char name[10];
  //FBuff<RGBA_32>* buff;
  void* buff;

  void assign(void* buff_ptr);

  PropertyBuffPtr();
  ~PropertyBuffPtr();
};

class PropertyFuncAdress {
 public:
  void* (*func)(void* arg);

  PropertyFuncAdress();
  ~PropertyFuncAdress();
};

struct Properties {
  List<PropertyInt> Ints;
  List<PropertyFloat> Floats;
  List<PropertyObjectPtr> Pointers_Obj;
  List<PropertyBuffPtr> Pointers_Buff;
  // List<PropertyFuncAdress> Funcs;
};