#pragma once

#include "Containers/List.h"
#include "Operator/Operator.h"
#include "Types.h"

class Object;
class UIItem;
struct OpThread;
class DeviceManager;
struct KeyMap;
struct UInputs;


struct UserInterface {

  // Visual Interface
  UIItem* UIroot;

  // Tactile Interface
  KeyMap* kmap;

  // Interlayer
  DeviceManager* sysh;

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