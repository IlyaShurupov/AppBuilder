#pragma once

#include "LinkedList.h"
#include "MathMacros.h"

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

class PropertyFuncAdress {
 public:
  void* (*func)(void* arg);

  PropertyFuncAdress();
  ~PropertyFuncAdress();
};

struct Properties {
  List<PropertyInt> Ints;
  List<PropertyFloat> Floats;
  // List<PropertyFuncAdress> Funcs;
};