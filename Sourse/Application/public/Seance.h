#pragma once

#include "Operator.h"
#include "RayCast.h"
#include "Types.h"
//#include "UIUILayout.h"
#include "Window.h"
#include "KeyMap.h"



struct Project {
  // MeshObjects, Cameras, Renders...
  List<Object> collection;

  // UI layout
  List<Window> windows;

  // save project
  void OnWrite();

  // load project
  void OnRead();

  Window* C_actWin();
};

struct Prefferences {
  //UserKeyMap key_map;
  List<Operator> operators;
  //List<Editor> editors;

  // save prefs
  void OnWrite();

  // load prefs
  void OnRead();
};

struct Seance {
  Seance(std::string* basePath);
  ~Seance();

  Prefferences prefferences;

  // Application project
  Project project;

  // All execute commands from UI (pointers to predefined operators)
  List<OpThread> op_threads;

  // Save current context
  void OnWrite(/*file path*/);

  // load context
  void OnRead(/*file path*/);
};