#pragma once

#include "Operator.h"
#include "RayCast.h"
#include "Types.h"
#include "UI.h"
#include "Window.h"

struct Project {
  // MeshObjects, Cameras, Renders...
  List<Object> collection;

  // UI layout
  List<Window> windows;

  // save project
  void OnWrite();

  // load project
  void OnRead();
};

struct Prefferences {
  // KeyMap;
  List<Operator> operators;
  List<Editor> editors;

  // save prefs
  void OnWrite();

  // load prefs
  void OnRead();
};

struct Seance {
  Seance();
  ~Seance();

  Prefferences prefferences;

  // Application project
  Project project;

  // All execute commands from UI (pointers to predefined operators)
  List<Operator> exec_queue;

  // Save current context
  void OnWrite(/*file path*/);

  // load context
  void OnRead(/*file path*/);
};