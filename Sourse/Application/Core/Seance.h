#pragma once

#include "Containers/List.h"
#include "Ops/Ops.h"

class Object;
class UIItem;
class Operator;
struct Str;
struct OpThread;
struct SysHandler;
struct KeyMap;
struct UInputs;

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

  Seance(Str* basePath);
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
  void OnRead(/*file path*/);
};