#pragma once

#include "LinkedList.h"

struct Project {
  // MeshObjects, Cameras, Renders...
  List<class Object> collection;

  // UI layout
  List<struct Window> windows;

  // save project
  void OnWrite();

  // load project
  void OnRead();

  struct Window* C_actWin();
};

struct Prefferences {
  //UserKeyMap key_map;
  List<struct Operator> operators;
  //List<Editor> editors;

  // save prefs
  void OnWrite();

  // load prefs
  void OnRead();
};

struct Seance {
  Seance(struct Str* basePath);
  ~Seance();

  Prefferences prefferences;

  // Application project
  Project project;

  // All execute commands from UI (pointers to predefined operators)
  List<struct OpThread> op_threads;

  // Save current context
  void OnWrite(/*file path*/);

  // load context
  void OnRead(/*file path*/);
};