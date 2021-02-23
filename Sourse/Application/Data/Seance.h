#pragma once

#include "Containers/List.h"
#include "Operator/Operator.h"

class Object;
class UIItem;
struct Str;
struct OpThread;
struct SysHandler;
struct KeyMap;
struct UInputs;
struct DataBlock;

struct UserInterface {

  // Visual Interface
  UIItem* UIroot;

  // Tactile Interface
  KeyMap* kmap;

  // Interlayer
  SysHandler* sysh;


  void Input(struct Seance& C);
  void Output();

  ~UserInterface();
};

struct Seance {

  Seance();
  ~Seance();

  // Mesh Objects, Cameras, Renders...
  List<Object> objects;
  
  // Functionality of an application
  Operators ops;

  // User Interface
  UserInterface ui;

  // UI requests
  List<OpThread> threads;

  // Save current seance
  void OnWrite(/*file path*/);

  // load Seance
  void OnRead(Str* basePath, UIItem* (*UIIFromStr)(Str* id, Operators* ops, DataBlock* paramsdb, DataBlock* uiidb));
};